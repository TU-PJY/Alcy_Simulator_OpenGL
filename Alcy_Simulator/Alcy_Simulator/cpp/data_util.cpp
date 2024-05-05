#include"../header/data_util.h"



std::vector<std::string> data_list = {
	"[play count]=",
	"[squeak count]=",
	"[touch time]=",
};


std::vector<std::string> data_list2 = {
	"play count",
	"squeak count",
	"touch time",
};


void create_save_file() {
	bool folder_new{};

	std::filesystem::path folderPath = std::filesystem::path(getenv("USERPROFILE")) / "Documents" / "Alcy Simulator data";
	if (!std::filesystem::exists(folderPath)) {
		folder_new = true;
		if (!std::filesystem::create_directory(folderPath))  exit(1);
	}

	if (folder_new) {
		std::filesystem::path filePath = folderPath / "data.txt";
		std::ofstream outputFile(filePath);

		// 세이브 파일 최초 생성 시 필요한 내용들을 파일에 쓰기
		if (outputFile.is_open())
			for(auto &s : data_list)
				outputFile << s << 0 << std::endl;

		else  exit(1);
	}
}



void update_save_file(const std::string& target, int value) {
	auto it = std::find(data_list2.begin(), data_list2.end(), target);
	if (it == data_list2.end())  exit(1);


	std::filesystem::path folderPath = std::filesystem::path(getenv("USERPROFILE")) / "Documents" / "Alcy Simulator data";
	if (!std::filesystem::exists(folderPath))  exit(1);

	std::filesystem::path filePath = folderPath / "data.txt";
	std::ifstream inputFile(filePath);
	if (!inputFile)  exit(1);

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
	std::ofstream outputFile(filePath);
	outputFile << newContent;
	outputFile.close();
}



int load_data(std::string target) {
	auto it = std::find(data_list2.begin(), data_list2.end(), target);
	if (it == data_list2.end())  exit(1);

	// 텍스트 파일 열기
	std::filesystem::path folderPath = std::filesystem::path(getenv("USERPROFILE")) / "Documents" / "Alcy Simulator data";
	if (!std::filesystem::exists(folderPath)) exit(1);
		
	std::filesystem::path filePath = folderPath / "data.txt";
	std::ifstream inputFile(filePath);
	if (!inputFile) exit(1);
		

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