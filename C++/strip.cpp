// corrupts a pe file

#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>

void strip(const char* filePath)
{
    std::fstream file(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    IMAGE_DOS_HEADER dosHeader;
    file.read(reinterpret_cast<char*>(&dosHeader), sizeof(dosHeader));

    if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
    {
        std::cerr << "Not a valid PE file." << std::endl;
        return;
    }

    file.seekg(dosHeader.e_lfanew, std::ios::beg);
    DWORD peSignature;
    file.read(reinterpret_cast<char*>(&peSignature), sizeof(peSignature));

    if (peSignature != IMAGE_NT_SIGNATURE)
    {
        std::cerr << "Not a valid PE file." << std::endl;
        return;
    }

    IMAGE_NT_HEADERS ntHeaders;
    file.read(reinterpret_cast<char*>(&ntHeaders), sizeof(ntHeaders));

    if (ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0)
    {
        std::cerr << "No import table found." << std::endl;
        return;
    }

    DWORD importTableRVA = ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    DWORD importTableSize = ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

    std::vector<char> importTable(importTableSize);
    file.seekg(importTableRVA, std::ios::beg);
    file.read(importTable.data(), importTableSize);

    std::fill(importTable.begin(), importTable.end(), 0);

    file.seekp(importTableRVA, std::ios::beg);
    file.write(importTable.data(), importTableSize);

    ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = 0;

    file.seekp(dosHeader.e_lfanew + sizeof(DWORD), std::ios::beg);
    file.write(reinterpret_cast<char*>(&ntHeaders), sizeof(ntHeaders));

    std::cout << "Import table stripped successfully." << std::endl;

    file.close();
}

int main(int argc, const char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path>" << std::endl;
        return 1;
    }

    strip(argv[1]);
    return 0;
}
