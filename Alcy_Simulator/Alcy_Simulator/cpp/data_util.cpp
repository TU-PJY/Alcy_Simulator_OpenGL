#include"../header/data_util.h"

// game version
#define VERSION 2


std::vector<std::string> text_list = {
	"[game version]=",
	"[play count]=",
	"[squeak count]=",
	"[touch time]=",
};


std::vector<std::string> data_list = {
	"game version",
	"play count",
	"squeak count",
	"touch time",
};


// data file ppath
std::filesystem::path folder_path = std::filesystem::path(getenv("USERPROFILE")) / "Documents" / "Alcy Simulator data";
std::filesystem::path file_path = folder_path / "data.txt";


void check_data_invalid() {
	bool folder_new{};

	if (!std::filesystem::exists(folder_path)) {
		folder_new = true;

		if (!std::filesystem::create_directory(folder_path))
			exit(1);
	}

	if (folder_new) {
		std::ofstream outputFile(file_path);

		// 세이브 파일 최초 생성 시 필요한 내용들을 파일에 쓰기
		if (outputFile.is_open())
			for (auto& s : text_list)
				outputFile << s << 0 << std::endl;
		else  
			exit(1);

		// save version number to data
		update_data("game version", VERSION);
	}
}



void check_data_version() {
	if (VERSION <= load_data("game version"))
		return;

	// 기존 값을 저장하는 백업 컨테이너
	std::vector<int> back_up{};

	// 기존 값 백업
	for (auto& d : data_list)
		back_up.push_back(load_data(d));


	// 파일에서 문자열 읽기
	if (!std::filesystem::exists(folder_path))
		exit(1);

	std::ifstream inputFile(file_path);
	if (!inputFile)
		exit(1);


	std::string line{};

	while (std::getline(inputFile, line)) {
		// 문자열을 파싱하여 key와 value 분리
		size_t pos = line.find('=');
		if (pos != std::string::npos)
			std::string str = line.substr(0, pos);
	}

	inputFile.close();


	std::ofstream outputFile(file_path);

	if (!outputFile)
		exit(1);

	// 모든 데이터를 0인 상태로 추가
	for (const auto& str : text_list)
		outputFile << str << "0\n";

	outputFile.close();


	// 기존 값 복원
	for (int i = 0; i < data_list.size(); ++i)
		update_data(data_list[i], back_up[i]);
	
	// 게임 버전 데이터 변경
	update_data("game version", VERSION);
}



void reset_data() {
	// 파일에서 문자열 읽기
	if (!std::filesystem::exists(folder_path))
		exit(1);

	std::ifstream inputFile(file_path);
	if (!inputFile)
		exit(1);


	std::string line{};

	while (std::getline(inputFile, line)) {
		// 문자열을 파싱하여 key와 value 분리
		size_t pos = line.find('=');
		if (pos != std::string::npos)
			std::string str = line.substr(0, pos);
	}

	inputFile.close();



	std::ofstream outputFile(file_path);

	if (!outputFile)
		exit(1);

	// 모든 데이터를 0인 상태로 추가
	for (const auto& str : text_list)
		outputFile << str << "0\n";

	outputFile.close();

	// 버전만 저장
	update_data("game version", VERSION);
}



void update_data(const std::string& target, int value) {
	auto it = std::find(data_list.begin(), data_list.end(), target);
	if (it == data_list.end())  
		exit(1);

	if (!std::filesystem::exists(folder_path))
		exit(1);

	std::ifstream inputFile(file_path);
	if (!inputFile)  
		exit(1);

	std::string fileContent;

	if (inputFile) {
		// 파일 내용 읽기
		fileContent = std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
		inputFile.close();
	}

	// 정규 표현식 패턴
	std::regex pattern(R"(\[([\w\s]+)\]=(\d+))");

	// 새로운 파일 내용 생성 또는 기존 파일 내용 업데이트
	std::smatch match;
	std::string newContent = fileContent;
	bool keyFound = false;

	// 파일 내용에서 키 검색 및 업데이트
	while (std::regex_search(fileContent, match, pattern)) {
		std::string variable = match[1];
		std::string valueStr = match[2];
		int currentValue = std::stoi(valueStr);

		if (variable == target) {
			// 찾은 키에 해당하는 값 업데이트
			std::string replaceStr = "[" + target + "]=" + std::to_string(value);
			newContent = std::regex_replace(newContent, std::regex("\\[" + target + "\\]=" + valueStr), replaceStr);
			keyFound = true;
			break;
		}

		// 다음 매치를 위해 검색 범위 갱신
		fileContent = match.suffix();
	}

	// 텍스트 파일 열고 새로운 내용으로 덮어쓰기
	std::ofstream outputFile(file_path);
	outputFile << newContent;
	outputFile.close();
}



int load_data(std::string target) {
	auto it = std::find(data_list.begin(), data_list.end(), target);
	if (it == data_list.end())  
		exit(1);

	// 텍스트 파일 열기
	if (!std::filesystem::exists(folder_path))
		exit(1);
		
	std::ifstream inputFile(file_path);
	if (!inputFile) 
		exit(1);
		

	int data_get{};

	std::string line;

	// 정규 표현식 생성
	std::regex pattern(R"(\[(\w+\s+\w+)\]=(\d+))");

	while (std::getline(inputFile, line)) {
		std::smatch match;

		// 정규 표현식 매칭
		if (std::regex_search(line, match, pattern)) {
			std::string variable = match[1];
			std::string value = match[2];

			if (variable == target) {
				data_get = stoi(value);
				break;
			}
		}
	}

	// 텍스트 파일 닫기
	inputFile.close();

	// 얻은 데이터 리턴
	return data_get;
}