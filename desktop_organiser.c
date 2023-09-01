#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>

struct ext_to_folder_map {
    char* ext;
    char* folder;
};

struct ext_to_folder_map map[] = {
        {"zip", "Compressed"},
        {"rar", "Compressed"},
        {"7z", "Compressed"},
        {"exe", "Programs"},
        {"msi", "Programs"},
        {"jpg", "Images"},
        {"png", "Images"},
        {"gif", "Images"},
        {"jpeg", "Images"},
        {"webp", "Images"},
        {"mp4", "Videos"},
        {"mkv", "Videos"},
        {"mp3", "Music"},
        {"pdf","Books"},
        {"ebook", "Books"},
        {"txt", "Texts"}
};

char *get_file_extension(char *filename) {
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

void make_directory(const char *name) {
    struct stat sb;
    if (stat(name, &sb) != 0 && !S_ISDIR(sb.st_mode)) {
#ifdef __linux__
        mkdir(name, 0700);
#else
        mkdir(name);
#endif
    }
}

char* ext_to_folder(const char* ext){
    for(int i = 0; i < 16; i++){
        if(strcmp(map[i].ext, ext) == 0){
            return map[i].folder;
        }
    }
    return "Others";
}

void arrange_files() {
    DIR *d;
    struct dirent *dir;
    d = opendir("./");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(strcmp(get_file_extension(dir->d_name), "lnk") != 0 && strcmp(get_file_extension(dir->d_name), "") != 0){
                char* folder = ext_to_folder(get_file_extension(dir->d_name));
                make_directory(folder);
                char new_path[512];
                sprintf(new_path, "%s/%s", folder, dir->d_name);
                rename(dir->d_name, new_path);
            }

        }
        closedir(d);
    }
}

int main() {
    arrange_files();
    return 0;
}
