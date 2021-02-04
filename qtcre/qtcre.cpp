#include "qtcre.h"

using std::addressof;
using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::string;
using std::swap;
using std::vector;
using std::wcout;

template<class T> T reverse_bytes(T& y) {
	T x = y;
	char* a = reinterpret_cast<char*>(addressof(x));
	for (char* b = a + sizeof x - 1; a < b; ++a, --b)
		swap(*a, *b);
	return x;
}

bool data_compare(const char* p_data, const char* b_mask, const char* sz_mask)
{
	for (; *sz_mask; ++sz_mask, ++p_data, ++b_mask)
		if (*sz_mask == 'x' && *p_data != *b_mask)
			return false;
	return (*sz_mask) == 0;
}

const char* find_pattern(const char* data, size_t length, const char* pattern, const char* mask) {

	for (size_t i = 0; i < length; i++)
		if (data_compare(data + i, pattern, mask))
			return data + i;
	return nullptr;
}

enum EntryFlags
{
	NoFlags = 0x00,
	Compressed = 0x01,
	Directory = 0x02
};

struct entry
{
	std::wstring name;
	uint16_t flags = 0;
	// if directory
	uint32_t children_count = 0;
	uint32_t children_offset = 0;
	// else
	uint16_t country = 0;
	uint16_t language = 0;
	uint32_t data_offset = 0;
	// endif
	uint64_t last_modified = 0;

};

void fileTree(const vector<entry>& entries, int index, int depth)
{
	const auto e = entries.at(index);
	
	// Print depth
	if (index != 0) {
		for (int i = 1; i < depth; i++)
		{
			cout << " ";
		}
		wcout << "- " << e.name << " [";
		if(e.flags & Directory)
		{
			cout << "dir, children: " << e.children_count;
		} else
		{
			cout << "file, country: " << e.country << ", language: " << e.language << ", data_offset: 0x" << hex << e.data_offset << dec << (e.flags & Compressed ? ", compressed" : "");
		}
		cout << ", last_modified: " << e.last_modified << "]" << endl;
	}
	

	for(auto i = e.children_offset; i < e.children_offset + e.children_count; i++)
	{
		fileTree(entries, i, depth + 1);
	}
	
}

int main(int argc, char** argv)
{
	cout << R"x(          __                        
  _______/  |_  ___________   ____  
 / ____/\   __\/ ___\_  __ \_/ __ \ 
< <_|  | |  | \  \___|  | \/\  ___/ 
 \__   | |__|  \___  >__|    \___  >
    |__|           \/            \/)x";
	
	cout << "   qtcre v1.0.0" << endl;

	if(argc < 3)
	{
		std::cout << endl << "usage: ./qtcre <executable path> <image base>" << endl;
		return 1;
	}
	
	const uint32_t image_base = std::strtol(argv[2], nullptr, 0) + 0x1400;
	const char* pattern =
		"\x68\x00\x00\x00\x00" // 68 28 cb 65 00
		"\x68\x00\x00\x00\x00" // 68 70 5c 6b 00
		"\x68\x00\x00\x00\x00" // 68 c0 5d 6b 00
		"\x6a\x02" // 6a 02
		"\xe8\x00\x00\x00\x00" // e8 16 78 1b 00
		"\x68\x00\x00\x00\x00" // 68 c0 2b 63 00
		"\xe8\x00\x00\x00\x00" // e8 07 ee 1b 00
		"\x83\xc4\x14" // 83 c4 14
		"\xc3"; // c3
	
	const char* mask = "x????x????x????xxx????x????x????xxxx";
	std::ifstream input(argv[1], std::ios::binary);
	
	input.seekg(0, std::ios::end);
	const auto input_length = input.tellg();
	input.seekg(0, std::ios::beg);
	std::vector<char> data(input_length);
	input.read(&data[0], input_length);

	const char* base = find_pattern(&data[0], input_length, pattern, mask);

	const auto resource_data_ptr = *reinterpret_cast<const uint32_t*>(base + 0x1) - image_base;
	const auto resource_name_ptr = *reinterpret_cast<const uint32_t*>(base + 0x6) - image_base;
	const auto resource_struct_ptr = *reinterpret_cast<const uint32_t*>(base + 0xb) - image_base;
	
	cout << endl << hex
		<< "resource_struct_ptr: 0x" << resource_struct_ptr << endl
		<< "resource_data_ptr: 0x" << resource_data_ptr << endl
		<< "resource_name_ptr: 0x" << resource_name_ptr << dec << endl << endl;

	std::vector<entry> entries;
	
	auto struct_ptr = resource_struct_ptr;
	auto count = 1;
	for (auto i = 0; i < count; i++)
	{
		entry entry;
		const auto name_offset = reverse_bytes(*reinterpret_cast<uint32_t*>(&data[struct_ptr]));
		struct_ptr += 4;

		{
			uint32_t name_ptr = resource_name_ptr + name_offset;
			const uint16_t name_length = reverse_bytes(*reinterpret_cast<uint16_t*>(&data[name_ptr]));
			name_ptr += 2;
			// const uint32_t name_hash = reverse_bytes(*(uint32_t*)&data[name_ptr]);
			name_ptr += 4;
			auto* test = new wchar_t[name_length + 1];
			for (int j = 0; j < name_length; j++)
			{
				*(reinterpret_cast<char*>(test) + j * 2) = data[name_ptr + 1];
				*(reinterpret_cast<char*>(test) + j * 2 + 1) = data[name_ptr];
				name_ptr += 2;
			}
			test[name_length] = 0;
			entry.name = test;
			delete[] test;
		}

		
		entry.flags = reverse_bytes(*reinterpret_cast<uint16_t*>(&data[struct_ptr]));
		struct_ptr += 2;
		if (entry.flags & Directory) {
			entry.children_count = reverse_bytes(*reinterpret_cast<uint32_t*>(&data[struct_ptr]));
			struct_ptr += 4;
			entry.children_offset = reverse_bytes(*reinterpret_cast<uint32_t*>(&data[struct_ptr]));
			struct_ptr += 4;
			const auto new_count = entry.children_offset + entry.children_count;
			if (new_count > count) count = new_count;
		} else
		{
			entry.country = reverse_bytes(*reinterpret_cast<uint16_t*>(&data[struct_ptr]));
			struct_ptr += 2;
			entry.language = reverse_bytes(*reinterpret_cast<uint16_t*>(&data[struct_ptr]));
			struct_ptr += 2;
			entry.data_offset = reverse_bytes(*reinterpret_cast<uint32_t*>(&data[struct_ptr]));
			struct_ptr += 4;
		}
		entry.last_modified = reverse_bytes(*reinterpret_cast<uint64_t*>(&data[struct_ptr]));
		struct_ptr += 8;
		entries.emplace_back(entry);
	}

	fileTree(entries, 0, 0);

	cout << endl;
	
	return 0;
}
