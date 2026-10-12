#include <linux/hid.h>

#define TUSB_HID_VID 0xcafe
#define TUSB_HID_PID 0x4004

static const struct hid_device_id tusb_hid_ids[] = {
	{HID_USB_DEVICE(TUSB_HID_VID, TUSB_HID_PID)},
	{ }
};
MODULE_DEVICE_TABLE(hid, tusb_hid_ids);

static int tusb_hid_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	int status;
	u8 cmd[4];

	dev_info(&hdev->dev, "TUSB HID Device is probed\n");

	status = hid_parse(hdev);
	if (status) {
		dev_err(&hdev->dev, "hid_parse failed with %d\n", status);
		return status;
	}

	status = hid_hw_start(hdev, 0);
	if (status) {
		dev_err(&hdev->dev, "hid_hw_start failed with %d\n", status);
		return status;
	}

	cmd[0] = 's';
	cmd[1] = 0;
	cmd[2] = 1;
	cmd[3] = 0;

	status = hid_hw_output_report(hdev, cmd, sizeof(cmd));
	dev_info(&hdev->dev, "hid_hw_output_report returned %d\n", status);

	return 0;
}

static void tusb_hid_remove(struct hid_device *hdev)
{
	int status;
	u8 cmd[4];
	
	dev_info(&hdev->dev, "TUSB HID Device is removed\n");

	cmd[0] = 's';
	cmd[1] = 0;
	cmd[2] = 0;
	cmd[3] = 0;

	status = hid_hw_output_report(hdev, cmd, sizeof(cmd));
	dev_info(&hdev->dev, "hid_hw_output_report returned %d\n", status);

	hid_hw_stop(hdev);
}

static struct hid_driver tusb_hid_driver = {
	.name = "hid-tusb",
	.id_table = tusb_hid_ids,
	.probe = tusb_hid_probe,
	.remove = tusb_hid_remove,
};

module_hid_driver(tusb_hid_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4Linux");
MODULE_DESCRIPTION("Hello World Linux HID Device Driver for tinyUSB hid device");



