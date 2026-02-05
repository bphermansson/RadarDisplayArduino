#pragma once
#include <HardwareSerial.h>

#include <Arduino.h>
#include <vector>
extern std::vector<byte> header;
extern std::vector<byte> footer;
extern byte sysModeParam;
extern HardwareSerial mySerial;
std::vector<byte> constructPackage(const std::vector<byte>& data);
std::vector<byte> enableCommandMode();
std::vector<byte> disableCommandMode();
std::vector<byte> readVersionNumber();
std::vector<byte> restartModule();
std::vector<byte> getConfigParams(const std::vector<byte>& params);
std::vector<byte> setConfigParams(const std::vector<byte>& params, const std::vector<int>& values);
std::vector<byte> sysMode(byte param);
std::vector<byte> getCommandResponse();
void sendCommand(const std::vector<byte>& command);
