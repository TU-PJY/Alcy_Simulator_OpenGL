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

		// ���̺� ���� ���� ���� �� �ʿ��� ������� ���Ͽ� ����
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
		// ���� ���� �б�
		fileContent = std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
		inputFile.close();
	}

	// ���� ǥ���� ����
	std::regex pattern(R"(\[([\w\s]+)\]=(\d+))");

	// ���ο� ���� ���� ���� �Ǵ� ���� ���� ���� ������Ʈ
	std::smatch match;
	std::string newContent = fileContent;
	bool keyFound = false;

	// ���� ���뿡�� Ű �˻� �� ������Ʈ
	while (std::regex_search(fileContent, match, pattern)) {
		std::string variable = match[1];
		std::string valueStr = match[2];
		int currentValue = std::stoi(valueStr);

		if (variable == target) {
			// ã�� Ű�� �ش��ϴ� �� ������Ʈ
			std::string replaceStr = "[" + target + "]=" + std::to_string(value);
			newContent = std::regex_replace(newContent, std::regex("\\[" + target + "\\]=" + valueStr), replaceStr);
			keyFound = true;
			break;
		}

		// ���� ��ġ�� ���� �˻� ���� ����
		fileContent = match.suffix();
	}

	// �ؽ�Ʈ ���� ���� ���ο� �������� �����
	std::ofstream outputFile(filePath);
	outputFile << newContent;
	outputFile.close();
}



int load_data(std::string target) {
	auto it = std::find(data_list2.begin(), data_list2.end(), target);
	if (it == data_list2.end())  exit(1);

	// �ؽ�Ʈ ���� ����
	std::filesystem::path folderPath = std::filesystem::path(getenv("USERPROFILE")) / "Documents" / "Alcy Simulator data";
	if (!std::filesystem::exists(folderPath)) exit(1);
		
	std::filesystem::path filePath = folderPath / "data.txt";
	std::ifstream inputFile(filePath);
	if (!inputFile) exit(1);
		

	int data_get{};

	std::string line;

	// ���� ǥ���� ����
	std::regex pattern(R"(\[(\w+\s+\w+)\]=(\d+))");

	while (std::getline(inputFile, line)) {
		std::smatch match;

		// ���� ǥ���� ��Ī
		if (std::regex_search(line, match, pattern)) {
			std::string variable = match[1];
			std::string value = match[2];

			if (variable == target) {
				data_get = stoi(value);
				break;
			}
		}
	}

	// �ؽ�Ʈ ���� �ݱ�
	inputFile.close();

	// ���� ������ ����
	return data_get;
}