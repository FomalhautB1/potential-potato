#include "find_cycles.h"

// Функция для подсчета циклов определенного размера
void count_cycles(const std::string& filename) {
    OpenBabel::OBConversion conv;
    OpenBabel::OBMol mol;

    // Устанавливаем формат ввода как XYZ
    if (!conv.SetInFormat("xyz")) {
        std::cerr << "Ошибка: не удалось установить формат XYZ." << std::endl;
        return;
    }

    // Открываем файл
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return;
    }

    // Чтение молекулы из файла
    if (!conv.Read(&mol, &input)) {
        std::cerr << "Ошибка чтения данных из файла XYZ" << std::endl;
        return;
    }

    // Закрываем файл
    input.close();

    // Получение всех циклов
    mol.FindSSSR();
    std::vector<OpenBabel::OBRing*> rings = mol.GetSSSR();

    // Подсчет циклов по размеру
    int cycles_4 = 0, cycles_5 = 0, cycles_6 = 0, cycles_7 = 0, cycles_8 = 0;
    for (auto ring : rings) {
        size_t ring_size = ring->Size();
        if (ring_size == 4) ++cycles_4;
        else if (ring_size == 5) ++cycles_5;
        else if (ring_size == 6) ++cycles_6;
        else if (ring_size == 7) ++cycles_7;
        else if (ring_size == 8) ++cycles_8;

        // Вывод номеров атомов в цикле
        std::cout << "Цикл из " << ring_size << " атомов: (";
        for (auto atom_idx : ring->_path) {
            auto* atom = mol.GetAtom(atom_idx);
            std::cout << atom->GetType() << atom->GetIdx() << " ";
        }
        std::cout << ")" << std::endl;
    }

    // Вывод итоговых результатов
    std::cout << "Количество циклов в молекуле из файла " << filename << ":\n";
    std::cout << "4-членных: " << cycles_4 << "\n";
    std::cout << "5-членных: " << cycles_5 << "\n";
    std::cout << "6-членных: " << cycles_6 << "\n";
    std::cout << "7-членных: " << cycles_7 << "\n";
    std::cout << "8-членных: " << cycles_8 << "\n";
}