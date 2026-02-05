//Function to construct the package to be sent to the module

#include <constructPackage.h>

std::vector<byte> header = {0xFD, 0xFC, 0xFB, 0xFA};
std::vector<byte> footer = {0x04, 0x03, 0x02, 0x01};

std::vector<byte> constructPackage(const std::vector<byte>& data) {
    std::vector<byte> package;

  // Add header
  package.insert(package.end(), header.begin(), header.end());

  // Data Length
  package.push_back(static_cast<byte>(data.size()));
  package.push_back(0x00);

  // Add data
  package.insert(package.end(), data.begin(), data.end());

  // Add footer
  package.insert(package.end(), footer.begin(), footer.end());

  return package;
}

//Function to Construct the command to enable the command mode
std::vector<byte> enableCommandMode() {
  std::vector<byte> data = {0xFF, 0x00, 0x01, 0x00};
  return constructPackage(data);
}

//Function to Construct the command to disable the command mode
std::vector<byte> disableCommandMode() {
  std::vector<byte> data = {0xFE, 0x00};
  return constructPackage(data);
}

//Function to Construct the command to read the version number
std::vector<byte> readVersionNumber() {
  std::vector<byte> data = {0x00, 0x00};
  return constructPackage(data);
}

//Function to Construct the command to restart the module
std::vector<byte> restartModule() {
  std::vector<byte> data = {0x68, 0x00};
  return constructPackage(data);
}

//Function to Construct the command to get the configuration parameters
std::vector<byte> getConfigParams(const std::vector<byte>& params) {
  std::vector<byte> data = {0x08, 0x00};
  
  for (byte b : params) {
    data.push_back(b);  
    data.push_back(0x00);
  }

  return constructPackage(data);
}

//Function to Construct the command to set the configuration parameters
std::vector<byte> setConfigParams(const std::vector<byte>& params, const std::vector<int>& values) {
    std::vector<byte> data = {0x07, 0x00}; // Initialize with specific header bytes

    for (size_t i = 0; i < params.size(); i++) {
        // Add the parameter byte
        data.push_back(params[i]);
        data.push_back(0x00);

        // Add the 4 bytes for the corresponding value (little-endian)
        int value = values[i];
        for (int j = 0; j < 4; j++) {
            byte byteValue = (value >> (8 * j)) & 0xFF; // Little-endian: LSB first
            data.push_back(byteValue);
        }

        // Add padding byte after each value
    }

    return constructPackage(data); // Return the constructed package
}

//Function to Construct the command to set the system mode
std::vector<byte> sysMode(byte param) {
    std::vector<byte> data = {0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    data.insert(data.begin() + 4, param);
    sysModeParam = param;
    return constructPackage(data);
}

//Function to send the command to the module
void sendCommand(const std::vector<byte>& command) {
  mySerial.write(command.data(), command.size());
  Serial.print("Sent: ");
  for (byte b : command) {
    Serial.print(b, HEX);
    Serial.print(" ");
  }
  Serial.println();
}

//Function to get the response of the command
std::vector<byte> getCommandResponse() {
  std::vector<byte> response;

  // Wait for data to be available
  delay(100); // Adjust the delay as needed

  if (mySerial.peek() == 0xFD) {
    while (mySerial.available()) {
      response.push_back(mySerial.read());
      if (response.size() >= 4 && response[response.size() - 4] == 0x04 && response[response.size() - 3] == 0x03 && response[response.size() - 2] == 0x02 && response[response.size() - 1] == 0x01) {
        break;
      }
    }
  }

  if (response.size() > 0) {
    Serial.print("Received: ");
    for (byte b : response) {
      Serial.print(b, HEX);
      Serial.print(" ");
    }
    Serial.println();

    if (response.size() > 7) {
      Serial.print("Message Status: ");
      if (response[7] == 0x01) {
        Serial.println("Success");
      } else {
        Serial.println("Unsuccessful Command");
      }
    } else {
      Serial.println("Error: Response too short");
    }
  } else {
    Serial.println("No response received");
  }

  return response;
}