#include "API.h"
#define DATA_SIZE 10

API::API(String baseUrl):_baseUrl(baseUrl){}

bool API::begin(String user, String password) {
  bool authBool = auth(user, password);
  bool dataBool = lastData();
  return authBool && dataBool; 
}

bool API::auth(String user, String password) {
  HTTPClient http;
  http.begin(_baseUrl + "/auth/");
  http.setAuthorization(user.c_str(), password.c_str());
  
  http.setReuse(true);
  http.setTimeout(_timeout);
  http.GET();
  String response = http.getString();

  StaticJsonDocument<1500> doc;
  DeserializationError error = deserializeJson(doc, response.c_str());

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return false;
  }

  const int userId = doc["user"]["id"];
  const char* token = doc["token"];
  const char* refresh = doc["refresh"];

  _userId = userId;
  _token = token;
  _refresh = refresh;

  http.end();
  return true;
}

bool API::sendData(int sensorId, int typeId, float* values) {
  HTTPClient http;   
  http.begin(_baseUrl+"/sensors/data");

  _lastSetId[typeId]++;

  String req;
  String base = "{\"shortOut\":true, \"sensorId\":" + String(sensorId) + ", \"userId\":" + String(_userId) + ", \"setId\":" + String(_lastSetId[typeId]) + ", ";

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Token", _token);

  req = base + "\"typeId\":"+ typeId +", \"setSeq\":0, \"value\":\"";
  for(int idx = 0; idx < DATA_SIZE; idx++) {
    req += String(values[idx], 2);
    if(idx < DATA_SIZE - 1) req += + ", ";  
  }
  req += "\"}";

  http.setReuse(true);
  http.setTimeout(_timeout);
  int httpResponseCode = http.POST(req.c_str());
  String response = http.getString();

  Serial.println(http.errorToString(httpResponseCode));
  Serial.println(httpResponseCode);
  Serial.println(response);

  http.end();
  return true;
}

bool API::lastData() {
  HTTPClient http;
  http.begin(_baseUrl + "/sensors/" + _userId + "/data?lastSet=true");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Token", _token);
  http.setReuse(true);
  http.setTimeout(_timeout);

  http.GET();
  String response = http.getString();

  StaticJsonDocument<5000> doc;
  DeserializationError error = deserializeJson(doc, response.c_str());

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return false;
  }

  const int lastSetHR = doc["HR"]["0"][0]["setId"];
  const int lastSetSpO2 = doc["SpO2"]["0"][0]["setId"];

  _lastSetId[0] = lastSetHR;
  _lastSetId[1] = lastSetSpO2;

  http.end();
  return true;
}