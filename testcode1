#!/usr/bin/python3

import time
import math
from pymavlink import mavutil

# connect to the flight controller
master = mavutil.mavlink_connection('tcp:127.0.0.1:5762')

# wait for a heartbeat
master.wait_heartbeat()

# inform user
print("Connected to system:", master.target_system, ", component:", master.target_component)

#Arm
master.mav.command_long_send(
    master.target_system,
    master.target_component,
mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM,
    0, # confirmation
    1, # param1 (1 to indicate arm)
    0, # param2 (all other params meaningless)
    0, # param3
    0, # param4
    0, # param5
    0, # param6
    0) # param7

#wait until arming confirmed (can manually check with master.motors_armed())
print("Waiting for the vehicle to arm")
master.motors_armed_wait()
print('Armed!')

import time
wait_time = 10
time.sleep(wait_time)

def request_message_interval(master, message_input: str, frequency_hz: float):
    message_name = "MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT " + message_input
    message_id = getattr(mavutil.mavlink, message_name)
    master.mav.command_long_send(master.target_system, master.target_component, mavutil.mavlink.MAV_CMD_SET_MESSAGE_INTERVAL, 0, message_id, 1e6/ frequency_hz, 0, 0, 0, 0, 0)
    print("Requested the message successfully.")


def get_requested_data(master, message_name: str, dict_key: str, value_unit: str, save_name: str):
    try:
        message_index = 0
        dict1 = master.recv_match(type= message_name, blocking=True, timeout=0.1).to_dict()
        dict_value = dict1[dict_key]
        toWrite = "Message_Index, " + message_index + " :" + str(dict_value) + value_unit
        with open(save_name, 'a') as file:file.write(toWrite)
        file.write('\n')  
        message_index += 1
        request_message_interval(master, "VFR_HUD", 1)
    
        while True: 
            get_requested_data(master, "VFR_HUD", 'alt', "m", save_name)
    except:
        pass
