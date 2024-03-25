#include "intel_hex.h"

unsigned char calculate_checksum(const std::string& line) {
    uint8_t checksum = 0;
    for (size_t i = 1; i < line.length() - 2; i += 2) {
        checksum += std::stoi(line.substr(i, 2), nullptr, 16);
    }
    checksum = 0x100 - checksum;
    return checksum;
}

// Функция для парсинга строки в формате Intel Hex
std::vector<unsigned char> parse_intel_hex(const std::string& file_path) {
    std::vector<unsigned char> data;
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    std::string line;
    unsigned short code_address;

    while (std::getline(file, line)) // Ищем код
    {
        if (line.empty() || (line[0] != ':')) {
            continue; // Пропускаем пустые строки и строки без двоеточия в начале
        }

        // Разбор строки Intel Hex
        unsigned char byte_count = std::stoi(line.substr(1, 2), nullptr, 16);
        unsigned short address = std::stoi(line.substr(3, 4), nullptr, 16);
        unsigned char record_type = std::stoi(line.substr(7, 2), nullptr, 16);
        if (record_type == 3) {
            code_address = std::stoi(line.substr(13, 4), nullptr, 16);
            break;
        } else if (record_type == 1) {
            break;
        }
        unsigned char checksum = std::stoi(line.substr(line.length() - 2, 2), nullptr, 16);

        // Проверка контрольной суммы
        unsigned char calc_checksum = calculate_checksum(line);
        if (checksum != calc_checksum) {
            file.close();
            throw std::runtime_error("Invalid checksum");
        }
    }
    
    file.seekg(std::ios_base::beg);

    while (std::getline(file, line)) {
        if (line.empty() || line[0] != ':') {
            continue; // Пропускаем пустые строки и строки без двоеточия в начале
        }

        // Разбор строки Intel Hex

        unsigned char byte_count = std::stoi(line.substr(1, 2), nullptr, 16);
        unsigned short address = std::stoi(line.substr(3, 4), nullptr, 16);
        unsigned char record_type = std::stoi(line.substr(7, 2), nullptr, 16);
        if ((code_address != address) && (code_address != 0)) {
            continue;
        } else {
            code_address = 0;
        }
        std::vector<unsigned char> record_data;
        for (size_t i = 9; i < line.length() - 2; i += 2) {
            record_data.push_back(std::stoi(line.substr(i, 2), nullptr, 16));
        }
        unsigned char checksum = std::stoi(line.substr(line.length() - 2, 2), nullptr, 16);

        // Проверка контрольной суммы
        unsigned char calc_checksum = calculate_checksum(line);
        if (checksum != calc_checksum) {
            file.close();
            throw std::runtime_error("Invalid checksum");
        }

/*
        // Обработка типа записи
        if (record_type == 0) {
            data.insert(data.end(), record_data.begin(), record_data.end());
        } else if (record_type == 1) {
            break; // Конец файла
        } else {
            throw std::runtime_error("Unsupported record type");
        }
*/
        // Обработка типа записи
        if ((record_type == 1) || ((record_data.at(0) == 0x01) && (record_data.at(1) == 0x00) && (record_data.at(2) == 0x02) && (record_data.at(3) == 0x00))) {
            break;
        } else {
            data.insert(data.end(), record_data.begin(), record_data.end());
        }
    }
    file.close();
    return data;
}