#include "usb_lib.h"
#include "usb_desc.h"

// USB Standard Device Descriptor
const uint8_t Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
{
	JOYSTICK_SIZ_DEVICE_DESC,	// bLength: Size of this descriptor in bytes
	USB_DESCRIPTOR_TYPE_DEVICE,	// bDescriptorType: Specifies the descriptor type (0x01 - Device descriptor)
	0x00, 0x02,	// bcdUSB: Identifies the version of the USB specification that this descriptor structure complies with. This value is a binary-coded decimal number.
	0x00,		// bDeviceClass: Class Code (Assigned by USB Org). If equal to Zero, each interface specifies it’s own class code If equal to 0xFF, the class code is vendor specified. Otherwise field is valid Class Code.
	0x00,		// bDeviceSubClass: Specifies the subclass code of the device as assigned by the USB specification group.
	0x00,		// bDeviceProtocol: Specifies the protocol code of the device as assigned by the USB specification group.
	0x40,		// bMaxPacketSize0: Specifies the maximum packet size, in bytes, for endpoint zero of the device. The value must be set to 8, 16, 32, or 64.
	0x83, 0x04,	// idVendor: Specifies the vendor identifier for the device as assigned by the USB specification committee. (0x0483)
	0x10, 0x57, // idProduct: Specifies the product identifier. This value is assigned by the manufacturer and is device-specific. (0x5710)
	0x00, 0x02, // bcdDevice: Identifies the version of the device. This value is a binary-coded decimal number. (rel. 2.00)
	1,			// iManufacturer: Specifies a device-defined index of the string descriptor that provides a string containing the name of the manufacturer of this device.
	2,			// iProduct: Specifies a device-defined index of the string descriptor that provides a string that contains a description of the device.
	3,			// iSerialNumber: Specifies a device-defined index of the string descriptor that provides a string that contains a manufacturer-determined serial number for the device.
	0x01		// bNumConfigurations: Specifies the total number of possible configurations for the device.
};

// USB Configuration Descriptor All Descriptors (Configuration, Interface, Endpoint, Class, Vendor)
const uint8_t Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
{
	9,		// bLength: Size of this descriptor in bytes
	USB_DESCRIPTOR_TYPE_CONFIGURATION,	// bDescriptorType: Specifies the descriptor type (0x02 - Config descriptor)
	JOYSTICK_SIZ_CONFIG_DESC, 0x00,	// wTotalLength: Specifies the total length, in bytes, of all data for the configuration. The length includes all interface, endpoint, class, or vendor-specific descriptors that are returned with the configuration descriptor.
	0x01,	// bNumInterfaces: Specifies the total number of interfaces supported by this configuration.
	0x01,	// bConfigurationValue: Contains the value that is used to select a configuration. This value is passed to the USB SetConfiguration request , as described in version 1.1 of the Universal Serial Bus Specification. The port driver does not currently expose a service that allows higher-level drivers to set the configuration.
	0x00,	// iConfiguration: Specifies the device-defined index of the string descriptor for this configuration.
	0xE0,	// bmAttributes: Specifies a bitmap to describe behavior of this configuration. The bits are described and set in little-endian order.
	0x32,	// MaxPower: Specifies the power requirements of this device in two-milliampere units. This member is valid only if bit seven is set in bmAttributes. (100 mA: this current is used for detecting Vbus)

		// Descriptor of Joystick Mouse interface
		9,		// bLength: Size of this descriptor in bytes
		USB_DESCRIPTOR_TYPE_INTERFACE,	// bDescriptorType: Specifies the descriptor type (0x04 - Interface descriptor)
		0x00,	// bInterfaceNumber: The index number of the interface.
		0x00,	// bAlternateSetting: The index number of the alternate setting of the interface.
		0x01,	// bNumEndpoints: The number of endpoints that are used by the interface, excluding the default status endpoint.
		0x03,	// bInterfaceClass: The class code of the device that the USB specification group assigned. (0x03 - HID) (http://www.usb.org/developers/defined_class)
		0x01,	// bInterfaceSubClass: The subclass code of the device that the USB specification group assigned. (1=BOOT, 0=NO BOOT)
		0x02,	// bInterfaceProtocol: The protocol code of the device that the USB specification group assigned. (0=NONE, 1=KEYBOARD, 2=MOUSE)
		0,		// iInterface: The index of a string descriptor that describes the interface.

			// Descriptor of Joystick Mouse HID
			JOYSTICK_SIZ_HID_DESC,	// bLength: Numeric expression that is the total size of the HID descriptor.
			HID_DESCRIPTOR_TYPE,	// bDescriptorType: Constant name specifying type of HID descriptor. (0x21 - HID descriptor)
			0x10, 0x01,	// bcdHID: Numeric expression identifying the HID Class Specification release. (0x10 0x01 - HID Version 1.10, 0x00 0x01 - HID Version 1.00)
			0x00,		// bCountryCode: Numeric expression identifying country code of the localized hardware. (00=NOT LOCALIZED)
			0x01,		// bNumDescriptors: Numeric expression specifying the number of class descriptors (always at least one i.e. Report descriptor.)
			0x22,		// bDescriptorType: Constant name identifying type of class descriptor. (0x22 (Class=Report))
			JOYSTICK_SIZ_REPORT_DESC, 0x00,	// wDescriptorLength: Numeric expression that is the total size of the Report descriptor.

				// Descriptor of Joystick Mouse endpoint
				7,			// bLength: Specifies the length, in bytes, of this descriptor.
				USB_DESCRIPTOR_TYPE_ENDPOINT,	// bDescriptorType: Specifies the descriptor type. (0x05 - Endpoint descriptor)
				0x81,		// bEndpointAddress: Specifies the USB-defined endpoint address. The four low-order bits specify the endpoint number. (0x81 - IN)
				0x03,		// bmAttributes: The two low-order bits specify the endpoint type, one of USB_ENDPOINT_TYPE_CONTROL,
				0x04, 0x00,	// wMaxPacketSize: Specifies the maximum packet size that can be sent from or to this endpoint. (4 bytes)
				0x20		// bInterval: Polling Interval (32 ms)
};

const uint8_t Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] =
{
	0x05, 0x01,        // Usage Page (Generic Desktop)
	0x09, 0x02,        // Usage (Mouse)
	0xA1, 0x01,        // Collection (Application)
	0x09, 0x01,        //   Usage (Pointer)
	0xA1, 0x00,        //   Collection (Physical)
	0x05, 0x09,        //     Usage Page (Button)
	0x19, 0x01,        //     Usage Minimum (Button 1)
	0x29, 0x05,        //     Usage Maximum (Button 5)
	0x15, 0x00,        //     Logical Minimum (0)
	0x25, 0x01,        //     Logical Maximum (1)
	0x95, 0x05,        //     Report Count (5)
	0x75, 0x01,        //     Report Size (1)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non Volatile,Bit Field)
	0x95, 0x01,        //     Report Count (1)
	0x75, 0x03,        //     Report Size (3)
	0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non Volatile,Bit Field)
	0x05, 0x01,        //     Usage Page (Generic Desktop)
	0x09, 0x30,        //     Usage (X)
	0x09, 0x31,        //     Usage (Y)
	0x09, 0x38,        //     Usage (Wheel)
	0x15, 0x81,        //     Logical Minimum (-127)
	0x25, 0x7F,        //     Logical Maximum (127)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x03,        //     Report Count (3)
	0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position,Non Volatile,Bit Field)
	0xC0,              //   End Collection
	0xC0              // End Collection
};

// USB String Descriptors (optional)
const uint8_t Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
{
	JOYSTICK_SIZ_STRING_LANGID,
	USB_DESCRIPTOR_TYPE_STRING,
	0x09, 0x04 // LangID = 0x0409: U.S. English
};

const uint8_t Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
{
	JOYSTICK_SIZ_STRING_VENDOR,	// Size of Vendor string
	USB_DESCRIPTOR_TYPE_STRING,	// bDescriptorType
	'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,	// Manufacturer: "STMicroelectronics"
	'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0, 'c', 0, 's', 0
};

const uint8_t Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
{
	JOYSTICK_SIZ_STRING_PRODUCT,	// bLength
	USB_DESCRIPTOR_TYPE_STRING,		// bDescriptorType
	'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'J', 0,
	'o', 0, 'y', 0, 's', 0, 't', 0, 'i', 0, 'c', 0, 'k', 0
};

uint8_t Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
{
	JOYSTICK_SIZ_STRING_SERIAL,	// bLength
	USB_DESCRIPTOR_TYPE_STRING,	// bDescriptorType
	'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0
};
