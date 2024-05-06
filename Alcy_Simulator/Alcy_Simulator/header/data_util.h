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
#include <unordered_map>

void update_data(const std::string& target, int value);
int load_data(std::string target);

void check_data_invalid();
void check_data_version();
void reset_data();