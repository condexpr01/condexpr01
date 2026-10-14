#ifndef CORE_HEADER_FILESTRING_GUARD
#define CORE_HEADER_FILESTRING_GUARD

#include <fstream>
#include <string>

namespace core{

	using std::ifstream;
	using std::string;

	//load file to string
	class filestring{

		//error status
		private:
			string str{};
			bool status{false};
			const char* reason = nullptr;

		//get error status methods
		public:
			bool        is_ok() noexcept{return status;}
			const char* what()  noexcept{return reason;}

		//methods
		public:
			string& get_str() noexcept{return str;}

		//RAII
		public:
			filestring(const char* filepath) noexcept{
				if (filepath == nullptr){
					reason = "[filestring] filepath is nullptr";
					return;
				}

				ifstream file(filepath,std::ios_base::binary);

				if (!file.is_open()){
					reason = "[filestring] failed to open";
					return;
				}

				file.seekg(0,std::ios_base::end);
				if (file.fail()){
					reason = "[filestring] failed to seekg";
					return;
				}

				std::streamsize size = file.tellg();
				if (file.fail()){
					reason = "[filestring] failed to tellg";
					return;
				}

				file.seekg(0,std::ios_base::beg);
				if (file.fail()){
					reason = "[filestring] failed to seekg";
					return;
				}

				try{
					str.resize(size);
				}catch(std::bad_alloc&){
					reason = "[filestring] failed to resize string";
					return;
				}

				if (!file.read(str.data(),size)){
					reason = "[filestring] failed to read";
					return;
				}

				status = true;
			}

			filestring(const filestring&)  = default;
			filestring(filestring&&) = default;
			filestring& operator=(const filestring&) = default;
			filestring& operator=(filestring&&) = default;

			~filestring() = default;
	};

}

#endif
