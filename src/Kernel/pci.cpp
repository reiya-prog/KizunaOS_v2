#include "main.h"

namespace
{
    using namespace pci;
    uint32_t MakeAddress32(uint8_t bus, uint8_t device, uint8_t function, uint8_t register_addr)
    {
        // xをbitsだけ左シフトした値を返すラムダ式
        auto shl = [](uint32_t x, unsigned int bits)
        {
            return x << bits;
        };
        // 引数を成型して32bitアドレスを作る
        return shl(1, 31) | shl(bus, 16) | shl(device, 11) | shl(function, 8) | (register_addr & 0xfcu);
    }

    bool AddDevice(const Device &device)
    {
        if (num_device == devices.size())
            return false;
        devices[num_device] = device;
        ++num_device;
        return true;
    }
    bool ScanBus(uint8_t bus);
    bool ScanFunction(uint8_t bus, uint8_t dev, uint8_t function)
    {
        auto class_code = ReadClassCode(bus, dev, function);
        auto header_type = ReadHeaderType(bus, dev, function);
        Device device{bus, dev, function, header_type, class_code};
        if (!AddDevice(device))
            return false;
        if (class_code.Match(0x06u, 0x04u))
        {
            auto bus_numbers = ReadBusNumbers(bus, dev, function);
            uint8_t secondary_bus = (bus_numbers >> 8) & 0xffu;
            ScanBus(secondary_bus);
        }
        return true;
    }

    bool ScanDevice(uint8_t bus, uint8_t device)
    {
        if (!ScanFunction(bus, device, 0))
            return false;
        if (IsSingleFunctionDevice(ReadHeaderType(bus, device, 0)))
            return true;
        for (uint8_t function = 1; function < 8; ++function)
        {
            if (ReadVendorID(bus, device, function) == 0xffffu)
                continue;
            if (!ScanFunction(bus, device, function))
                return false;
        }
        return true;
    }

    bool ScanBus(uint8_t bus)
    {
        for (uint8_t device = 0; device < 32; ++device)
        {
            if (ReadVendorID(bus, device, 0) == 0xffffu)
                continue;
            if (!ScanDevice(bus, device))
                return false;
        }
        return true;
    }
}

namespace pci
{
    void WriteAddress(uint32_t address)
    {
        io_out32(kConfigAddress, address);
    }

    void WriteData(uint32_t value)
    {
        io_out32(kConfigData, value);
    }

    uint32_t ReadData()
    {
        return io_in32(kConfigData);
    }

    uint16_t ReadVendorID(uint8_t bus, uint8_t device, uint8_t function)
    {
        WriteAddress(MakeAddress32(bus, device, function, 0x00));
        return ReadData() & 0xffffu;
    }

    uint16_t ReadDeviceID(uint8_t bus, uint8_t device, uint8_t function)
    {
        WriteAddress(MakeAddress32(bus, device, function, 0x00));
        return ReadData() >> 16;
    }

    uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function)
    {
        WriteAddress(MakeAddress32(bus, device, function, 0x0c));
        return (ReadData() >> 16) & 0xffu;
    }

    ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function)
    {
        WriteAddress(MakeAddress32(bus, device, function, 0x08));
        auto reg = ReadData();
        ClassCode class_code;
        class_code.base = (reg >> 24) & 0xffu;
        class_code.sub = (reg >> 16) & 0xffu;
        class_code.interface = (reg >> 8) & 0xffu;
        return class_code;
    }

    uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function)
    {
        WriteAddress(MakeAddress32(bus, device, function, 0x18));
        return ReadData();
    }

    bool IsSingleFunctionDevice(uint8_t header_type)
    {
        return (header_type & 0x80u) == 0;
    }

    bool ScanAllBus()
    {
        num_device = 0;
        auto header_type = ReadHeaderType(0, 0, 0);
        if (IsSingleFunctionDevice(header_type))
            return ScanBus(0);
        for (uint8_t function = 0; function < 8; ++function)
        {
            if (ReadVendorID(0, 0, function) == 0xffffu)
                continue;
            if (!ScanBus(function))
                return false;
        }
        return true;
    }

    uint32_t ReadConfigRegister(const Device &device, uint8_t register_addr)
    {
        WriteAddress(MakeAddress32(device.bus, device.device, device.function, register_addr));
        return ReadData();
    }

    void WriteConfigRegister(const Device &device, uint8_t register_addr, uint32_t value)
    {
        WriteAddress(MakeAddress32(device.bus, device.device, device.function, register_addr));
        WriteData(value);
    }

    uint64_t ReadBar(Device &device, unsigned int bar_index)
    {
        if (bar_index >= 6)
            return 0;

        const auto addr = CalcBarAddress(bar_index);
        const auto bar = ReadConfigRegister(device, addr);

        if ((bar & 4u) == 0)
            return bar;
        if (bar_index >= 5)
            return 0;

        const auto bar_upper = ReadConfigRegister(device, addr + 4);

        return bar | (static_cast<uint64_t>(bar_upper) << 32);
    }
}