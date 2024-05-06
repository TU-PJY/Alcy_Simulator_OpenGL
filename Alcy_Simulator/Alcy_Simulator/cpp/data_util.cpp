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

		// ���̺� ���� ���� ���� �� �ʿ��� ������� ���Ͽ� ����
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

	// ���� ���� �����ϴ� ��� �����̳�
	std::vector<int> back_up{};

	// ���� �� ���
	for (auto& d : data_list)
		back_up.push_back(load_data(d));


	// ���Ͽ��� ���ڿ� �б�
	if (!std::filesystem::exists(folder_path))
		exit(1);

	std::ifstream inputFile(file_path);
	if (!inputFile)
		exit(1);


	std::string line{};

	while (std::getline(inputFile, line)) {
		// ���ڿ��� �Ľ��Ͽ� key�� value �и�
		size_t pos = line.find('=');
		if (pos != std::string::npos)
			std::string str = line.substr(0, pos);
	}

	inputFile.close();


	std::ofstream outputFile(file_path);

	if (!outputFile)
		exit(1);

	// ��� �����͸� 0�� ���·� �߰�
	for (const auto& str : text_list)
		outputFile << str << "0\n";

	outputFile.close();


	// ���� �� ����
	for (int i = 0; i < data_list.size(); ++i)
		update_data(data_list[i], back_up[i]);
	
	// ���� ���� ������ ����
	update_data("game version", VERSION);
}



void reset_data() {
	// ���Ͽ��� ���ڿ� �б�
	if (!std::filesystem::exists(folder_path))
		exit(1);

	std::ifstream inputFile(file_path);
	if (!inputFile)
		exit(1);


	std::string line{};

	while (std::getline(inputFile, line)) {
		// ���ڿ��� �Ľ��Ͽ� key�� value �и�
		size_t pos = line.find('=');
		if (pos != std::string::npos)
			std::string str = line.substr(0, pos);
	}

	inputFile.close();



	std::ofstream outputFile(file_path);

	if (!outputFile)
		exit(1);

	// ��� �����͸� 0�� ���·� �߰�
	for (const auto& str : text_list)
		outputFile << str << "0\n";

	outputFile.close();

	// ������ ����
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
	std::ofstream outputFile(file_path);
	outputFile << newContent;
	outputFile.close();
}



int load_data(std::string target) {
	auto it = std::find(data_list.begin(), data_list.end(), target);
	if (it == data_list.end())  
		exit(1);

	// �ؽ�Ʈ ���� ����
	if (!std::filesystem::exists(folder_path))
		exit(1);
		
	std::ifstream inputFile(file_path);
	if (!inputFile) 
		exit(1);
		

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