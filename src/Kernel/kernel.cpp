#include "main.h"

extern "C" void __cxa_pure_virtual() { io_hlt(); }

extern "C"
{
    void kernel_start(const FrameBuffer &frame_buffer)
    {
        kernel(frame_buffer);
    }
}

void kernel(const FrameBuffer &frame_buffer)
{
    switch (frame_buffer.pixel_format)
    {
    case kPixelRGB8BitPerColor:
        pixel_writer = new (pixel_writer_buf) RGBPixelWriter{frame_buffer};
        break;
    case kPixelBGR8BitPerColor:
        pixel_writer = new (pixel_writer_buf) BGRPixelWriter{frame_buffer};
        break;
    }
    for (int y = 0; y < frame_buffer.vertical_resolution; ++y)
    {
        for (int x = 0; x < frame_buffer.horizontal_resolution; ++x)
        {
            pixel_writer->Write(x, y, {255, 255, 255});
        }
    }
    console = new (console_buf) Console{*pixel_writer, {0, 0, 0}, {255, 255, 255}};

    for (int i = 0; i < frame_buffer.horizontal_resolution; ++i)
    {
        for (int j = 0; j < frame_buffer.vertical_resolution; ++j)
        {
            pixel_writer->Write(i, j, 0x20b2aaLL);
        }
    }
    printk("Linking to you, access our connection!\n\tKizunaOS, boot up!\n");

    if (!pci::ScanAllBus())
        return;

    for (int ni = 0; ni < pci::num_device; ++ni)
    {
        const auto &dev = pci::devices[ni];
        auto vendor_id = pci::ReadVendorID(dev);
        auto class_code = pci::ReadClassCode(dev.bus, dev.device, dev.function);
        printk("%d.%d.%d: vend %04x, class %08x, head %02x\n", dev.bus, dev.device, dev.function, vendor_id, class_code, dev.header_type);
    }

    pci::Device *xhc_dev = nullptr;
    for (int ni = 0; ni < pci::num_device; ++ni)
    {
        if (pci::devices[ni].class_code.Match(0x0cu, 0x03u, 0x30u))
        {
            xhc_dev = &pci::devices[ni];
            if (0x8086 == pci::ReadVendorID(*xhc_dev))
                break;
        }
    }

    const auto xhc_bar = pci::ReadBar(*xhc_dev, 0);
    const uint64_t xhc_mmio_base = xhc_bar & ~static_cast<uint64_t>(0xf);

    usb::xhci::Controller xhc{xhc_mmio_base};


    io_hlt();
}