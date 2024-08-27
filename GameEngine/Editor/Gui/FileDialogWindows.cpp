#include "FileDialogWindows.h"

std::string FileDialogWindows::ShowFileDialog(const FileDialogOption& option)
{
    std::string path = "";

    // Initialize the COM library.
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileDialog;

        if (option.dialogType == FileDialogType::SAVE_DIALOG)
        {
            hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileDialog));
        }
        else
        {
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileDialog));
        }

        if (SUCCEEDED(hr))
        {
            // Set options to allow folder selection
            DWORD dwOptions;
            hr = pFileDialog->GetOptions(&dwOptions);

            if (SUCCEEDED(hr))
            {
                if(option.returnType == FileDialogReturnType::PICK_FOLDER)
                    pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

                std::vector<COMDLG_FILTERSPEC> fileTypes;
                for (const auto& filter : option.filters) {
                    fileTypes.push_back({ filter.first.c_str(), filter.second.c_str() });
                }

                hr = pFileDialog->SetFileTypes(static_cast<UINT>(fileTypes.size()), fileTypes.data());

                // Show the Open dialog box.
                hr = pFileDialog->Show(NULL);

                // Get the folder name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pFileDialog->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR pszFolderPath;

                        // Get the display name of the folder.
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);

                        // Display the folder path to the user.
                        if (SUCCEEDED(hr))
                        {
                            int bufferSize = WideCharToMultiByte(CP_UTF8, 0, pszFolderPath, -1, nullptr, 0, nullptr, nullptr);
                            std::string result(bufferSize - 1, '\0');
                            WideCharToMultiByte(CP_UTF8, 0, pszFolderPath, -1, &result[0], bufferSize, nullptr, nullptr);
                            
                            path = result;

                            CoTaskMemFree(pszFolderPath);
                        }

                        pItem->Release();
                    }
                }
            }

            // Release the FileOpenDialog object.
            pFileDialog->Release();
        }

        // Uninitialize the COM library.
        CoUninitialize();
    }

    std::replace(path.begin(), path.end(), '\\', '/');
    return path;
}