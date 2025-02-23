#include "HttpServer.h"

AsyncWebServer server(80);

// HTML web page to handle input fields including topics
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      font-family: Arial, sans-serif;
      flex-direction: column;
    }
    form {
      text-align: center;
      padding: 20px;
      border: 1px solid #ccc;
      border-radius: 8px;
    }
    input[type="text"] {
      padding: 8px;
      margin: 10px;
      width: 200px;
      border: 1px solid #ccc;
      border-radius: 4px;
    }
    input[type="submit"] {
      padding: 8px 16px;
      background-color: #4CAF50;
      color: white;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }
    input[type="submit"]:hover {
      background-color: #45a049;
    }
    .extra-fields {
      display: none; /* Initially hidden */
    }
  </style>
  <script>
    function toggleBrokerFields() {
      var anonymousChecked = document.getElementById("anonymous").checked;
      var brokerFields = document.getElementById("brokerFields");
      var input6 = document.getElementById("input6");

      // Set the value of input6 based on the checkbox state
      input6.value = anonymousChecked ? "true" : "false"; // 'true' if checked, 'false' if unchecked

      if (anonymousChecked) {
        brokerFields.style.display = "none"; // Hide fields if anonymous is checked
      } else {
        brokerFields.style.display = "block"; // Show fields if anonymous is unchecked
      }
    }
  </script>
</head>
<body>
  <form action="/submit" method="GET">
    SSID: <input type="text" name="input1"><br>
    Password: <input type="text" name="input2"><br>
    Broker ip:port: <input type="text" name="input3"><br>

    <!-- Anonymous checkbox -->
    Anonymous: <input type="checkbox" id="anonymous" checked onclick="toggleBrokerFields()"><br>

    <!-- Broker fields, initially hidden if Anonymous is checked -->
    <div id="brokerFields" class="extra-fields">
      Broker Username: <input type="text" name="input4"><br>
      Broker Password: <input type="text" name="input5"><br>
    </div>

    <!-- Topics field -->
    Topics (set topics in oreder 'temp:dev1:dev2'): <input type="text" name="topics"><br>

    <!-- Hidden input to pass the checkbox state -->
    <input type="hidden" id="input6" name="input6" value="true">

    <input type="submit" value="Submit">
  </form>
</body>
</html>
)rawliteral";

void runHttpServer(MemoryHandler& memoryHandler) {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/submit", HTTP_GET, [&memoryHandler](AsyncWebServerRequest *request) {
    String input1 = request->hasParam("input1")
                        ? request->getParam("input1")->value()
                        : "N/A";
    String input2 = request->hasParam("input2")
                        ? request->getParam("input2")->value()
                        : "N/A";
    String input3 = request->hasParam("input3")
                        ? request->getParam("input3")->value()
                        : "N/A";
    String input4 = request->hasParam("input4")
                        ? request->getParam("input4")->value()
                        : "N/A";
    String input5 = request->hasParam("input5")
                        ? request->getParam("input5")->value()
                        : "N/A";                        
    String input6 = request->hasParam("input6")
                        ? request->getParam("input6")->value()
                        : "true";  // Default to true if the param is not present
    String topics = request->hasParam("topics")
                        ? request->getParam("topics")->value()
                        : "N/A";

    // Convert input6 to a boolean (true or false)
    bool isAnonymous = (input6 == "true");

    // Now pass input6 and topics to the writeCredentials function
    memoryHandler.writeCredentials(input1, input2, input3, input4, input5, isAnonymous, topics);

    // Combine inputs into a single string
    String combinedInputs =
        "SSID: " + input1 + ", Password: " + input2 + ", Broker_addr: " + input3 + ", Broker_usr: " + input4 + ", Broker_pass: " + input5 + ", Topics: " + topics + ", Anonymous: " + (isAnonymous ? "true" : "false");
    Serial.println(combinedInputs);

    request->send(200, "text/html",
      "<html>"
      "<head>"
      "<style>"
      "body {"
      "  font-family: Arial, sans-serif;"
      "  background-color: #f4f4f4;"
      "  color: #333;"
      "  margin: 0;"
      "  padding: 0;"
      "  text-align: center;"
      "}"
      "h1 {"
      "  color: #4CAF50;"
      "  font-size: 36px;"
      "  margin-top: 50px;"
      "}"
      "p {"
      "  font-size: 18px;"
      "  margin: 20px;"
      "  line-height: 1.6;"
      "}"
      "a {"
      "  color: #ffffff;"
      "  background-color: #4CAF50;"
      "  padding: 10px 20px;"
      "  text-decoration: none;"
      "  border-radius: 5px;"
      "  font-size: 18px;"
      "}"
      "a:hover {"
      "  background-color: #45a049;"
      "}"
      "footer {"
      "  margin-top: 40px;"
      "  font-size: 14px;"
      "  color: #777;"
      "}"
      "</style>"
      "</head>"
      "<body>"
      "<h1>Received Inputs</h1>"
      "<p>" + combinedInputs + "</p>"
      "<p>Now you can restart ESP32, or rewrite configuration.</p>"
      "<a href=\"/\">Return to Home Page</a>"
      "</body>"
      "</html>");

  });

  server.begin();
}
