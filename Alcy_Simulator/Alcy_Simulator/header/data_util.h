#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <regex>
#include <vector>
#include <iostream>
#include <map>

void create_save_file();
void update_save_file(const std::string& target, int value);
int load_data(std::string target);