==================================================
How to use USB
==================================================

Unfortunately, PCs used in EDIT2013 does not mount USB automatically.
So please mount USB manually.

:Sample command:

.. code-block:: bash

    $ dmesg
    $ su (ask passwd to lecturer)
    # mount /dev/sdc1 /mnt/usb (or /media/usb)
    # ls /mnt/usb
    # ... (rsync or cp files)
    # umount /mnt/usb
    # exit
