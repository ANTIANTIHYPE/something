#include <Windows++.h>

// ??? TODO: fix

void myWMIObjectCallback ( IWbemClassObject * pclsObj, 
                           LPVOID userData )
{
    VARIANT vtProp;
    VariantInit(&vtProp);
    HRESULT hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
    if ( SUCCEEDED(hr) )
    {
        wprintf(L"Name: %s\n", vtProp.bstrVal);
        VariantClear(&vtProp);
    }
    else
    {
        printf("Failed to get property. Error code 0x%lX\n", hr);
    }
}

int main()
{
    CSTR query = "SELECT * FROM Win32_OperatingSystem";
    sendWQLQuery(query, myWMIObjectCallback, NULL);

    return 0;
}
