# -*- coding: utf-8 -*-

import pyautogui
pyautogui.FAILSAFE = False

import ctypes
ctypes.windll.user32.LockWorkStation()