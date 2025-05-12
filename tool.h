#ifndef TOOL_H
#define TOOL_H

#include <qDebug>

#include <QCoreApplication>

#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <QThread>

HHOOK keyboardHook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //qDebug()<<nCode<<" "<<wParam<<Qt::endl;
    if (nCode >= 0)
    {
        return 1; // 禁用按键
    }

    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void DisableKeyboard()
{
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (!keyboardHook)
    {
        qDebug()<< "Failed to set keyboard hook." << Qt::endl;
    }
}


bool SetMute(int level)
{
    HRESULT hr;
    IMMDeviceEnumerator* pDeviceEnumerator=0;
    IMMDevice* pDevice=0;
    IAudioEndpointVolume* pAudioEndpointVolume=0;

    try{
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator);
        if(FAILED(hr)) throw "CoCreateInstance";
        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice);
        if(FAILED(hr)) throw "GetDefaultAudioEndpoint";
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume);
        if(FAILED(hr)) throw "pDevice->Active";

        if(level==0){
            hr = pAudioEndpointVolume->SetMute(FALSE,NULL);
            if(FAILED(hr)) throw "SetMute";
        }else if(level==1){
            hr = pAudioEndpointVolume->SetMute(TRUE,NULL);
            if(FAILED(hr)) throw "SetMute";
        }
    }
    catch(...){
        if(pAudioEndpointVolume) pAudioEndpointVolume->Release();
        if(pDevice) pDevice->Release();
        if(pDeviceEnumerator) pDeviceEnumerator->Release();
        throw;
    }
    return false;
}

void SetMasterVolume(float volume) {
    HRESULT hr;
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pAudioEndpointVolume = NULL;

    // CoInitialize是必须的
    hr = CoInitialize(NULL);

    // 初始化IMMDeviceEnumerator接口
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);

    // 获取默认的渲染设备
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

    // 激活IAudioEndpointVolume接口
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndpointVolume);

    // 设置音量
    hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(volume, NULL);

    qDebug()<<hr<<Qt::endl;
    // 释放接口
    pAudioEndpointVolume->Release();
    pDevice->Release();
    pEnumerator->Release();
    CoUninitialize();
}

class MyThread : public QThread
{
public:
    void run() override
    {
        while(1)
        {
            RECT rect;
            rect.top=rect.left=0;
            rect.bottom=rect.right=1;
            ClipCursor(&rect);
            SetCursorPos(0,1);
            SetMasterVolume(1.0f);
            Sleep(10);
        }
    }
};

#endif // TOOL_H






