#!/usr/bin/env python3


import customtkinter
import os
from PantherUI import PantherUI
def home():
    return os.path.expanduser("~")

if __name__ == '__main__':
    customtkinter.set_appearance_mode('System')
    #customtkinter.set_default_color_theme(SD_THEME_PATH)
    app = PantherUI()
    app.mainloop()