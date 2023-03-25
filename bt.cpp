#pragma region includes
#include <iostream>

#include <string>

#include <algorithm>    // remove_if
#include <locale>       //codecvt_utf8
#include <codecvt>

#include <windows.h>    // WCHAR

#include "bluetoothapis.h"
// #include <bthsdpdef.h>
// #include <bthdef.h>
// #include <tchar.h>

// to compile: g++ bt.cpp -o bt.exe -lbthprops -static
//#pragma comment(lib, "Bthprops.lib")
// "C:\Program Files (x86)\Windows Kits\8.1\Include\um\bluetoothapis.h"

using namespace std;
#pragma endregion includes

HANDLE get_radio() {
    /* get first bluetooth radio, will be called by get devices */
    BLUETOOTH_FIND_RADIO_PARAMS radioParam;
    radioParam.dwSize = sizeof(BLUETOOTH_FIND_RADIO_PARAMS);

    HANDLE hRadio;
    HBLUETOOTH_RADIO_FIND hFindRadio = BluetoothFindFirstRadio(&radioParam, &hRadio);
    // do{
    //     wprintf(L"Found radio\n");      
    // }while(BluetoothFindNextRadio(&radioParam, &hRadio));    
    BluetoothFindRadioClose(hFindRadio);
    return hRadio; 
  }

string convert_wstr(WCHAR szName[BLUETOOTH_MAX_NAME_SIZE]){
    /* convert wide char array to narrow string, keep only alphanum and spaces */
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    std::wstring szNameWstr(szName);
    std::string s = converter.to_bytes( szNameWstr );
    s.erase(std::remove_if(s.begin(), s.end(),
        []( auto const& c ) -> bool { return !std::isalnum(c) && c !=' '; } ), s.end());

    return s;
}


BLUETOOTH_DEVICE_INFO get_devices(string device_name){
    /* https://learn.microsoft.com/en-us/windows/win32/api/bluetoothapis/nf-bluetoothapis-bluetoothfindfirstdevice */
    BLUETOOTH_DEVICE_SEARCH_PARAMS srch;
    srch.fReturnAuthenticated = TRUE;
    srch.fReturnRemembered = TRUE;
    srch.fReturnConnected = TRUE;
    srch.fReturnUnknown = TRUE;
    srch.fIssueInquiry = TRUE;
    srch.cTimeoutMultiplier = 2;
    srch.hRadio = get_radio();
    srch.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);

    BLUETOOTH_DEVICE_INFO btdi;
    btdi.dwSize = sizeof(btdi);
    
    HBLUETOOTH_DEVICE_FIND hFind = BluetoothFindFirstDevice(&srch, &btdi);
    do {            
        string s = convert_wstr(btdi.szName);
        if(s == device_name){
            std::cout << s << " matched" << endl;
            break;
        } else {
            std::cout << s << " not matched" << endl;
        }
    } while(BluetoothFindNextDevice(hFind, &btdi));

    BluetoothFindDeviceClose(hFind);
    return btdi;
}

void set_service_state(string device_name){
    
    HANDLE radio = get_radio();
    BLUETOOTH_DEVICE_INFO deviceInfo = get_devices(device_name);
    GUID AudioSinkServiceClass_UUID = { 0x0000110B,0x0000,0x1000,0x80,0x00,0x00,0x80,0x5F,0x9B,0x34,0xFB };
    GUID HandsfreeServiceClass_UUID = { 0x0000111E,0x0000,0x1000,0x80,0x00,0x00,0x80,0x5F,0x9B,0x34,0xFB };

    // const uint BLUETOOTH_SERVICE_DISABLE = 0;
    // const uint BLUETOOTH_SERVICE_ENABLE = 0x00000001;

    // disconnect
    BluetoothSetServiceState(radio,&deviceInfo,&AudioSinkServiceClass_UUID,0);
    BluetoothSetServiceState(radio,&deviceInfo,&HandsfreeServiceClass_UUID,0);

    // then connect
    BluetoothSetServiceState(radio,&deviceInfo,&AudioSinkServiceClass_UUID,1);
    BluetoothSetServiceState(radio,&deviceInfo,&HandsfreeServiceClass_UUID,1);
}

int main(int argc, char *argv[]) 
{
    if(argc < 2) {
        printf("missing device name");
        return 0;
    }
    // get_radio();
    // get_devices(argv[1]);
    set_service_state(argv[1]);
    return 0;
}