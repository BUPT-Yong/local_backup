# -*- coding: utf-8 -*-

import pyautogui
import time
import pyperclip
import requests
from datetime import datetime
import schedule
import traceback
import os

max_retry_times = 20

root_path = os.path.dirname(os.path.abspath(__file__))
pic_dir = os.path.join(root_path, "autoGuiImg")

print(pic_dir)

def get_news():
    url = "https://v1.hitokoto.cn"
    params = {"c": "d", "encode": "json"} 
    r = requests.get(url, params=params)
    json = r.json()
    return json.get("hitokoto"), json.get("from_who"), json.get("from")

def build_msg():
  myTime = time.localtime()
  if myTime.tm_hour < 14 and myTime.tm_hour > 6:
    msg = "Good morning and love you tons! \n\n"
  else:
    msg = "Nice Dream and love you tons! \n\n"

  content, who, where = get_news()
  msg += "每日一句:\n"
  msg += content + " "
  if who is None:
     msg += " --《" + str(where) + "》"
  else:
    msg += "  --" + str(who) + " * 《" + str(where) + "》"
  
  return msg

def write_success_log(msg):
  lof_file = open(os.path.join(root_path, "all_in_one.log"), 'a+')
  now = datetime.now() # current date and time
  lof_file.write(now.strftime("%Y-%m-%d, %H:%M:%S") + " send success!")
  lof_file.write(msg)
  lof_file.write("\n*****************************************************\n")
  lof_file.close()

def write_failed_log(error_str):
  lof_file = open(os.path.join(root_path, "all_in_one.log"), 'a+')
  now = datetime.now() # current date and time
  lof_file.write(now.strftime("%Y-%m-%d, %H:%M:%S") + " send failed!\n")
  lof_file.write(error_str)
  lof_file.write("\n*****************************************************\n")
  lof_file.close()

def open_wechat() -> bool:
  try:
    pyautogui.hotkey('winleft','d')
    pos1 = pyautogui.locateCenterOnScreen(os.path.join(pic_dir, "wechat_OPEN.png"), confidence=0.6)
    try_time = 0
    while pos1 is None and try_time < max_retry_times:
      pyautogui.hotkey('winleft','d')
      pos1 = pyautogui.locateCenterOnScreen(os.path.join(pic_dir,"wechat_OPEN.png"), confidence=0.6)
      time.sleep(0.5)
      try_time+=1
    if try_time >= max_retry_times and pos1 is None:
      write_failed_log("open wechat failed")
      return False
    pyautogui.doubleClick(pos1)
  except Exception as e:
    write_failed_log("open wechat exception")
    write_failed_log(str(e))
    return False
  return True

def try_login() -> bool:
  try:
    log_in_pos = pyautogui.locateCenterOnScreen(os.path.join(pic_dir, "login.png"), confidence=0.4)
    print(log_in_pos)
    if log_in_pos is not None:
      pyautogui.click(log_in_pos)
      pyautogui.sleep(10)
    
    log_in_confirm = pyautogui.locateCenterOnScreen(os.path.join(pic_dir, "log_in_confirm.png"), confidence=0.4)
    try_time = 0
    while log_in_confirm is not None and try_time < max_retry_times:
      pyautogui.sleep(10)
      log_in_confirm = pyautogui.locateCenterOnScreen(os.path.join(pic_dir, "log_in_confirm.png"), confidence=0.4)
      try_time+=1
    
    if try_time >= max_retry_times and log_in_confirm is not None:
      write_failed_log("login not confirmed")
      return False 
  except Exception as e:
    write_failed_log("log in exception")
    write_failed_log(str(e))
    return False
  return True

def find_lulu() -> bool:
  try:
    pos2 = pyautogui.locateCenterOnScreen(os.path.join(pic_dir,  "lulu_icon.png"), confidence=0.7)
    try_time = 0
    while pos2 is None and try_time < max_retry_times:
      pos3 = pyautogui.locateCenterOnScreen(os.path.join(pic_dir, "wechat_leaf_side.png"), confidence=0.6)
      if pos3 is not None:
        pyautogui.moveTo(pos3.x+100, pos3.y)
        pyautogui.scroll(20000, x=pos3.x+100, y=pos3.y)
      pos2 = pyautogui.locateCenterOnScreen(os.path.join(pic_dir, "lulu_icon.png"), confidence=0.7)
      time.sleep(0.5)
      try_time += 1
    
    if try_time >= max_retry_times and pos2 is None:
      write_failed_log("find lulu_icon failed")
      return False
    pyautogui.click(pos2)
  except Exception as e:
    write_failed_log("find lulu_icon exception")
    write_failed_log(str(e))
    return False
  return True

def click_send() -> bool:
  try:
    pos4 = pyautogui.locateCenterOnScreen(os.path.join(pic_dir, "send.png"), confidence=0.6)
    if pos4 is None:
      write_failed_log("click send failed")
      return False
    pyautogui.click(pos4)
  except Exception as e:
    write_failed_log("click send exception")
    write_failed_log(str(e))
    return False
  return True


def input_msg() -> str:
  try:
    pyautogui.hotkey('ctrl','a')
    pyautogui.press("del")

    msg = build_msg()
    pyperclip.copy(msg)
    pyautogui.hotkey('ctrl','v')
    return msg
  except Exception as e:
    write_failed_log("input msg exception")
    write_failed_log(str(e))
  return None

def run():
  try:
    if open_wechat() and try_login() and find_lulu():
      msg = input_msg()
      if click_send():
        write_success_log(msg)
  except Exception as e:
    write_failed_log(str(e))

schedule.every().day.at("09:30").do(run)
schedule.every().day.at("23:00").do(run)

while True:
  schedule.run_pending()
  time.sleep(1)