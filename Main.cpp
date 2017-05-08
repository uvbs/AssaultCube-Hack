/*
	This is a very very simplistic hack for AssaultCube.
	It is made by following the youtube video series by 'Guided Hacking'
	as I got pretty bored doing nothing so I wanted to learn something new.

	It works on servers too but please don't use this hack online, this is just
	for educational purposes! :)

	The functions:
	- Infinite Ammo
	- Infinite Health / HP

	(Yes, very simple)

	How to use:
	- Open up the .exe (or start it in debug mode in Visual Studio)
	- Press F1 / F2 to use a certain cheat
	- Press the 'Insert'-Key to close the application


	~Piorjade @ 2017
*/

#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>


DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD offsets[], DWORD BaseAddress);
void WriteToMemory(HANDLE hProcHandle);



std::string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
std::string GameStatus;

bool IsGameAvail;
bool UpdateOnNextRun;

//AMMO VARS
bool AmmoStatus;
BYTE AmmoValue[] = {0xA3, 0x1C, 0x0, 0x0};
DWORD AmmoBaseAddress = { 0x00509B74 };
DWORD AmmoOffsets[] = { 0x384, 0x14, 0x0 };

//HEALTH VARS
bool HealthStatus;
BYTE HealthValue[] = { 0x39, 0x5, 0x0, 0x0 };
DWORD HealthBaseAddress = { 0x00509B74 };
DWORD HealthOffsets[] = { 0xF8 };

int main()
{
	// Main Loop
	HWND hGameWindow = NULL;
	int timeSinceLastUpdate = clock();
	int GameAvailTMR = clock();
	int onePressTMR = clock();

	DWORD dwProcID = NULL;
	HANDLE hProcHandle = NULL;
	UpdateOnNextRun = true;
	std::string sAmmoStatus = "OFF";
	std::string sHealthStatus = "OFF";

	// While not pressing the insert key
	while (!GetAsyncKeyState(VK_INSERT))
	{
		if (clock() - GameAvailTMR > 100)
		{
			GameAvailTMR = clock();
			IsGameAvail = false;
			
			hGameWindow = FindWindowA(NULL, LGameWindow);

			if (hGameWindow)
			{
				GetWindowThreadProcessId(hGameWindow, &dwProcID);
				if (dwProcID != 0)
				{
					hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
					if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL)
					{
						GameStatus = "Failed to open process for valid handle.";
					}
					else
					{
						GameStatus = GameName + " ready to hack.";
						IsGameAvail = true;
					}
				}
				else
				{
					GameStatus = "Failed to get process ID.";
				}
			}
			else
			{
				GameStatus = GameName + " not found.";
			}

			if (UpdateOnNextRun || clock() - timeSinceLastUpdate > 5000)
			{
				system("cls");
				std::cout << "----------------------------------------------" << std::endl;
				std::cout << "- Thank you for using my hack! :)            -" << std::endl;
				std::cout << "- ~ Piorjade @ 2017                          -" << std::endl;
				std::cout << "----------------------------------------------" << std::endl;
				std::cout << "GAME STATUS: " << GameStatus << std::endl;
				std::cout << "[F1] Unlimited Ammo -> " << sAmmoStatus << " <- " << std::endl << std::endl;
				std::cout << "[F2] Unlimited HP   -> " << sHealthStatus << " <- " << std::endl << std::endl;
				std::cout << "[INSERT] Exit" << std::endl;
				UpdateOnNextRun = false;
				timeSinceLastUpdate = clock();
			}

			if (IsGameAvail)
			{
				// WRITE TO MEMORY
				WriteToMemory(hProcHandle);
			}

		}

		if (clock() - onePressTMR > 400)
		{
			if (IsGameAvail)
			{
				// AMMO
				if (GetAsyncKeyState(VK_F1))
				{
					onePressTMR = clock();
					AmmoStatus = !AmmoStatus;
					UpdateOnNextRun = true;
					if (AmmoStatus) sAmmoStatus = "ON";
					else sAmmoStatus = "OFF";
				}
				// HEALTH
				else if (GetAsyncKeyState(VK_F2))
				{
					onePressTMR = clock();
					HealthStatus = !HealthStatus;
					UpdateOnNextRun = true;
					if (HealthStatus) sHealthStatus = "ON";
					else sHealthStatus = "OFF";
				}
			}
		}
	}
	//CloseHandle(hProcHandle);
	//CloseHandle(hGameWindow);

	return ERROR_SUCCESS;
}

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD offsets[], DWORD BaseAddress)
{
	// Find the dynamic address of the specified value
	DWORD pointer = BaseAddress;
	DWORD pTemp;

	DWORD pointerAddr;
	for (int c = 0; c < PointerLevel; c++)
	{
		if (c == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL);

		}
		pointerAddr = pTemp + offsets[c];
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL);
	}
	return pointerAddr;
}

void WriteToMemory(HANDLE hProcHandle)
{
	// Write the new values to the address
	DWORD AddressToWrite;
	if (AmmoStatus)
	{
		AddressToWrite = FindDmaAddy(3, hProcHandle, AmmoOffsets, AmmoBaseAddress);
		WriteProcessMemory(hProcHandle, (BYTE*)AddressToWrite, &AmmoValue, sizeof(AmmoValue), NULL);
	}
	if (HealthStatus)
	{
		AddressToWrite = FindDmaAddy(1, hProcHandle, HealthOffsets, HealthBaseAddress);
		WriteProcessMemory(hProcHandle, (BYTE*)AddressToWrite, &HealthValue, sizeof(HealthValue), NULL);
	}
}