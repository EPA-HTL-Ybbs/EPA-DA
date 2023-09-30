#! /usr/bin/python3

import serial
import influxdb_client, os, time
from dotenv import load_dotenv

from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

load_dotenv()

ser = serial.Serial('COM5', 115200)

token = os.environ.get("INFLUXDB_TOKEN")
org = "EPA"
url = "http://localhost:8086"

write_client = influxdb_client.InfluxDBClient(url=url, token=token, org=org)
bucket="ballin"
write_api = write_client.write_api(write_options=SYNCHRONOUS)

scanning = False
flag = False
currentTime = 0
scanType = "wlan" #change to "" once esp gives types
macCount = 0

while True:
    line = ser.readline().decode().strip()

    if line == "=====":
        scanning = True
        flag = True
        currentTime = time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime())
        macCount = 0
        continue

    if line == "-----":
        if flag == True:
            scanning = False
            point = (Point(measurement_name=scanType)
                     .field(field="macAdresses", value=macCount)
                     .time(time=currentTime, write_precision=WritePrecision.S)
            )
            write_api.write(bucket=bucket, org="EPA", record=point)
            print(f"{currentTime}: {macCount}")

    if scanning:
        try:
            mac, rssi = line.split(sep='/')
            #point = (Point(measurement_name=scanType)
            #         .field(field="mac", value=mac)
            #         .field(field="rssi", value=rssi)
            #         .time(time=currentTime, write_precision=WritePrecision.S)
            #)
            #write_api.write(bucket=bucket, org="EPA", record=point)
            macCount += 1
        except ValueError:
            print(f"Invalid line: {line}")
            continue