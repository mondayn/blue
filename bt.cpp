#pragma region includes
#include <iostream>
//#include <stdint.h>
//using namespace std;
// #pragma once
// #include <winapifamily.h>
#include <windows.h>
#include <bthsdpdef.h>
#include <bthdef.h>
#include "bluetoothapis.h"
// #include <tchar.h>
// #include <strsafe.h>
//#pragma comment(lib, "Bthprops.lib")
// "C:\Program Files (x86)\Windows Kits\8.1\Include\um\bluetoothapis.h"
#pragma endregion includes


HANDLE get_radio() {
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


BLUETOOTH_DEVICE_INFO get_devices(){

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
        wprintf(L"%S\n",btdi.szName);
    //   BOOL BluetoothFindNextDevice(
    //   HBLUETOOTH_DEVICE_FIND hFind,
    //   BLUETOOTH_DEVICE_INFO  *pbtdi
    } while(BluetoothFindNextDevice(hFind, &btdi));
    BluetoothFindDeviceClose(hFind);
    return btdi;
}


void get_services() {
//     DWORD BluetoothEnumerateInstalledServices(
//   HANDLE                      hRadio,
//   const BLUETOOTH_DEVICE_INFO *pbtdi,
//   DWORD                       *pcServiceInout,
//   GUID                        *pGuidServices
// );
    // DWORD pcServiceInout;
    // GUID pGuidServices[16];
    // BLUETOOTH_DEVICE_INFO pbtdi = get_devices();
    // BluetoothEnumerateInstalledServices(
    //     get_radio()
    //     ,&pbtdi
    //     ,&pcServiceInout
    //     ,pGuidServices
    // );
    // wprintf(L"Services found %d",pcServiceInout);
    // for(int i=0;i<6;i++){
    //     wprintf(L"%S\n",pGuidServices[i]);
    //     // std::cout<< pGuidServices[i] << ' ';
    // }

    //from bthdef.h
    // DEFINE_GUID(AudioSinkServiceClass_UUID,0x0000110B,0x0000,0x1000,0x80,0x00,0x00,0x80,0x5F,0x9B,0x34,0xFB);
    //DEFINE_GUID(HandsfreeServiceClass_UUID,0x0000111E,0x0000,0x1000,0x80,0x00,0x00,0x80,0x5F,0x9B,0x34,0xFB);
    // wprintf(L"%S",pGuidServices);

// btcom -b %airpods% -r -s110b
// btcom -b %airpods% -r -s111e

    // wprintf(L"done printing guid services");
    // BLUETOOTH_DEVICE_INFO *pbtdi,
    // DWORD *pcServices,
    // GUID *pGuidServices
}


void set_service_state(){
    //     private const uint BLUETOOTH_SERVICE_DISABLE = 0;
// private const uint BLUETOOTH_SERVICE_ENABLE = 0x00000001;
    
// DWORD BluetoothSetServiceState(
//   HANDLE                      hRadio,
//   const BLUETOOTH_DEVICE_INFO *pbtdi,
//   const GUID                  *pGuidService,
//   DWORD                       dwServiceFlags
// );
      
//       BluetoothSetServiceState(pointer.Handle, ref deviceInfo, ref serviceRef, 0);
// BluetoothSetServiceState(pointer.Handle, ref deviceInfo, ref serviceRef, 1);

}

int main(int argc, char *argv[]) {

    wprintf(L"num args=%d",argc);
    // wprintf(L"args=%S",argv[1]);

    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    // get_radio();
    // get_devices();
    // get_services();
    //set_service_state();
    wprintf(L"done");
    return 0;
}