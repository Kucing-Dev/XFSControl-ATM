#include <Windows.h>
#include <xfsapi.h>
#include <xfscashdisp.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "msxfs.lib")

void logMessage(const std::string& message) {
    std::ofstream logFile("atm_log.txt", std::ios_base::app);
    logFile << message << std::endl;
}

int main() {
    HRESULT hResult;
    HSERVICE hService;
    DWORD dwTimeout = 10000;

    // Init XFS
    hResult = WFSStartUp(MAKEWORD(3, 0), nullptr);
    if (FAILED(hResult)) {
        logMessage("WFSStartUp failed: " + std::to_string(hResult));
        return 1;
    }

    hResult = WFSOpen("ATMCDM", NULL, NULL, 0, WFS_INDEFINITE_WAIT, &hService);
    if (FAILED(hResult)) {
        logMessage("WFSOpen failed: " + std::to_string(hResult));
        WFSCleanUp();
        return 1;
    }

    // Dapatkan status
    LPWFSRESULT lpResult = nullptr;
    hResult = WFSGetStatus(hService, &lpResult, dwTimeout);
    if (FAILED(hResult) || lpResult == nullptr) {
        logMessage("WFSGetStatus failed: " + std::to_string(hResult));
        WFSClose(hService);
        WFSFreeResult(lpResult);
        WFSCleanUp();
        return 1;
    }

    // Ambil data status dari lpResult
    auto* status = static_cast<LPWFSCDMSTATUS>(lpResult->lpBuffer);
    if (status->fwDevice == WFS_CDM_DEVONLINE) {
        logMessage("Device online.");
    } else {
        logMessage("Device not ready. Status: " + std::to_string(status->fwDevice));
        WFSFreeResult(lpResult);
        WFSClose(hService);
        WFSCleanUp();
        return 1;
    }

    WFSFreeResult(lpResult); // Bebaskan memori hasil status

    // Siapkan request dispense
    WFSCDMDISPENSE dispense{};
    dispense.usMixNumber = 1;
    dispense.fwPosition = WFS_CDM_POSFRONT;
    dispense.usTellerID = 0;

    LPWFSRESULT lpDispenseResult = nullptr;

    logMessage("Starting dispense...");

    hResult = WFSExecute(hService, WFS_CMD_CDM_DISPENSE, &dispense, dwTimeout, &lpDispenseResult);
    if (FAILED(hResult)) {
        logMessage("WFSExecute DISPENSE failed: " + std::to_string(hResult));
    } else {
        logMessage("Dispense success.");
    }

    if (lpDispenseResult)
        WFSFreeResult(lpDispenseResult);

    WFSClose(hService);
    WFSCleanUp();

    logMessage("Done.");

    return 0;
}
