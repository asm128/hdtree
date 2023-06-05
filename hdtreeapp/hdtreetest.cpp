#include "hdtree.h"
#include <Windows.h>

// Linking to the console subsystem executes a test on the tree object instead of calling WinMain() and launching the window
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/);

int main () {
	hd::MaterialTree		tree;

	constexpr const char*	testStrings[] = 
		{ "1"
		, "c"
		, "Branding"
		, "apppatch"
		, "AppReadiness"
		, "Containers"
		, "CSC"
		, "Cursors"
		, "DDACLSys.log"
		, "debug"
		, "Ascd_ProcessLog.ini"
		, "Ascd_tmp.ini"
		, "audio.log"
		, "bcastdvr"
		, "CbsTemp"
		, "comsetup.log"
		, "diagerr.xml"
		, "diagnostics"
		, "DiagTrack"
		, "diagwrn.xml"
		, "GameBarPresenceWriter"
		, "DigitalLocker"
		, "cab"
		, "DirectX.log"
		, "Downloaded Installations"
		, "gethelp_audiotroubleshooter_latestpackage.zip"
		, "glide2x.dll"
		, "Help"
		, "INF"
		, "bb"
		, "acb"
		, "bbc"
		, "b"
		, "glide3x.dll"
		, "a"
		, "acb"
		, "InfusedApps"
		, "system.ini"
		, "InputMethod"
		, "DPINST.LOG"
		, "OCR"
		, "DtcInstall.log"
		, "ehome"
		, "rescache"
		, "en"
		, "es-ES"
		, "Globalization"
		, "L2Schemas"
		, "LAN.log"
		, "LiveKernelReports"
		, "en-US"
		, "IME"
		, "RemotePackages"
		, "ImmersiveControlPanel"
		, "SKB"
		, "WMSysPr9.prx"
		, "SoftwareDistribution"
		, "write.exe"
		};

#define print_text_test_result(index, call, text) printf("\n%i -> %i | %s with %s", index, (call), #call, text);

	printf("\n\n--- Adding categories:\n");
	for(uint32_t iString = 0; iString < std::size(testStrings); ++iString) {
		std::string categoryName = testStrings[iString];
		print_text_test_result(iString, tree.AddCategory(categoryName), categoryName.c_str());
	}

	printf("\n\n--- Adding materials:\n");
	for(uint32_t iCategory = 0; iCategory < tree.Categories.size(); ++iCategory) {
		const std::string & categoryName = tree.Categories[tree.Categories.size() - 1 - iCategory].Name;
		for(uint32_t iString = 0; iString < std::size(testStrings); ++iString) {
			std::string materialName = (iCategory & 1) ? testStrings[std::size(testStrings) - 1 - iString] : testStrings[iString];
			print_text_test_result(iString, tree.AddMaterial(categoryName, materialName), materialName.c_str());
		}
	}

	printf("\n\n--- Category results:\n");
	for(auto cat : tree.Categories)
		printf("\n- %s", cat.Name.c_str());

	printf("\n\n--- Material results:\n");
	for(auto cat : tree.Categories) {
		printf("\n - Category: %s", cat.Name.c_str());
		for(auto mat : cat.Materials)
			printf("\n\t- %s", mat.c_str());
	}

	::WinMain(GetModuleHandle(0), 0, 0, 0);
	return EXIT_SUCCESS;
}
