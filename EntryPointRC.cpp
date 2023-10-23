#include "MemoryAPI.hpp"
#include "StringHelper.hpp"
#include "ExceptionTracer.hpp"
#include "zTypes.hpp"
#include "Int3Hook.hpp"
#include "FunctionHelper.hpp"
#include "DllLoader.hpp"
#include "FileNotifier.hpp"
#include "FileScanner.hpp"

ZN_STATUS DllFileCallback(std::wstring path)
{
	std::unique_ptr<DllLoader> m_pDllLoader = std::make_unique<DllLoader>(path.c_str());
	return m_pDllLoader->ExecuteDllFunction();
}

void HackyWayUnwindXD() {
	const std::wstring path(L"C:\\Users\\aleja\\Desktop\\Plugins\\");
	//std::unique_ptr< FileScanner> m_pFileScanner = std::make_unique< FileScanner>(path);
//	m_pFileScanner->ScanFiles(); //ScanFiles once on program start-up
	std::unique_ptr< EventNotification> m_pNotif = std::make_unique<EventNotification>(path);

	while (true) {
		m_pNotif->MonitorFilesAction(DllFileCallback);
	}
}

int main(void)
{
	__try {
		HackyWayUnwindXD();
	}
	__except (ExceptionFilter(GetExceptionCode(), GetExceptionInformation())) {}//Trace exception information including debug registers
	std::cin.get();
	return 0;
}