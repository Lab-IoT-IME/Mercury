#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class API
{
  public:
    API(String baseUrl);
  
    // Access methods
    bool auth(String user, String password);
    bool refresh();
    bool lastData();
    bool sendData(int sensorId, int typeId, float* values);
    bool begin(String user, String password);

    // Get endpoints
    String getEndpointAuth();
    String getEndpointRefresh();
    String getEndpointReceive();
    String getEndpointSend();

  private:
    String _token;
    String _refresh;
    String _baseUrl;
    int _lastSetId[2];
    int _datasize;
    int _timeout = 60000;
    int _userId;
};