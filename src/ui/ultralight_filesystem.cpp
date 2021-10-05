#include "ultralight_filesystem.h"

bool let::ultralight_filesystem::FileExists(const ultralight::String16 &path) {
    ZoneScopedN("filesystem::file_exists");
    const auto utf8_ultralight = ultralight::String(path).utf8();
    return std::filesystem::exists(std::string(utf8_ultralight.data()));
}

bool let::ultralight_filesystem::GetFileSize(ultralight::FileHandle handle, int64_t &result) {
    ZoneScopedN("filesystem::get_file_size");
    if (const auto file = _files.find(handle); file != _files.end()) {
        result = file->second.file.size();
        return true;
    }
    return false;
}

bool let::ultralight_filesystem::GetFileMimeType(const ultralight::String16 &path, ultralight::String16 &result) {
    ZoneScopedN("filesystem::get_file_mime_type");
    // Todo, create a lookup table here
    const auto string = std::string(ultralight::String(path).utf8().data());

    const auto view = std::string_view(string);

    if (view.ends_with(".aac")) result = "audio/aac";
    if (view.ends_with(".abw")) result = "application/x-abiword";
    if (view.ends_with(".arc")) result = "application/x-freearc";
    if (view.ends_with(".avi")) result = "video/x-msvideo";
    if (view.ends_with(".azw")) result = "application/vnd.amazon.ebook";
    if (view.ends_with(".bin")) result = "application/octet-stream";
    if (view.ends_with(".bmp")) result = "image/bmp";
    if (view.ends_with(".bz")) result = "application/x-bzip";
    if (view.ends_with(".bz2")) result = "application/x-bzip2";
    if (view.ends_with(".cda")) result = "application/x-cdf";
    if (view.ends_with(".csh")) result = "application/x-csh";
    if (view.ends_with(".css")) result = "text/css";
    if (view.ends_with(".csv")) result = "text/csv";
    if (view.ends_with(".doc")) result = "application/msword";
    if (view.ends_with(".docx")) result = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    if (view.ends_with(".eot")) result = "application/vnd.ms-fontobject";
    if (view.ends_with(".epub")) result = "application/epub+zip";
    if (view.ends_with(".gz")) result = "application/gzip";
    if (view.ends_with(".gif")) result = "image/gif";
    if (view.ends_with(".html")) result = "text/html";
    if (view.ends_with(".htm")) result = "text/html";
    if (view.ends_with(".ics")) result = "image/vnd.microsoft.icon";
    if (view.ends_with(".jar")) result = "text/calendar";
    if (view.ends_with(".jpeg")) result = "application/java-archive";
    if (view.ends_with(".jpg")) result = "image/jpeg";
    if (view.ends_with(".js")) result = "text/javascript";
    if (view.ends_with(".json")) result = "application/json";
    if (view.ends_with(".jsonld")) result = "application/ld+json";
    if (view.ends_with(".mid")) result = "audio/midi";
    if (view.ends_with(".midi")) result = "audio/x-midi";
    if (view.ends_with(".mjs")) result = "text/javascript";
    if (view.ends_with(".mp3")) result = "audio/mpeg";
    if (view.ends_with(".mp4")) result = "video/mp4";
    if (view.ends_with(".mpeg")) result = "video/mpeg";
    if (view.ends_with(".mpkg")) result = "application/vnd.apple.installer+xml";
    if (view.ends_with(".odp")) result = "application/vnd.oasis.opendocument.presentation";
    if (view.ends_with(".ods")) result = "application/vnd.oasis.opendocument.spreadsheet";
    if (view.ends_with(".odt")) result = "application/vnd.oasis.opendocument.text";
    if (view.ends_with(".oga")) result = "audio/ogg";
    if (view.ends_with(".ogv")) result = "video/ogg";
    if (view.ends_with(".ogx")) result = "application/ogg";
    if (view.ends_with(".opus")) result = "audio/opus";
    if (view.ends_with(".otf")) result = "font/otf";
    if (view.ends_with(".png")) result = "image/png";
    if (view.ends_with(".pdf")) result = "application/pdf";
    if (view.ends_with(".php")) result = "application/x-httpd-php";
    if (view.ends_with(".ppt")) result = "application/vnd.ms-powerpoint";
    if (view.ends_with(".pptx")) result = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    if (view.ends_with(".rar")) result = "application/vnd.rar";
    if (view.ends_with(".rtf")) result = "application/rtf";
    if (view.ends_with(".sh")) result = "application/x-sh";
    if (view.ends_with(".svg")) result = "image/svg+xml";
    if (view.ends_with(".swf")) result = "application/x-shockwave-flash";
    if (view.ends_with(".tar")) result = "application/x-tar";
    if (view.ends_with(".tif")) result = "image/tiff";
    if (view.ends_with(".tiff")) result = "image/tiff";
    if (view.ends_with(".ts")) result = "video/mp2";
    if (view.ends_with(".ttf")) result = "font/ttf";
    if (view.ends_with(".txt")) result = "text/plain";
    if (view.ends_with(".vsd")) result = "application/vnd.visio";
    if (view.ends_with(".wav")) result = "audio/wav";
    if (view.ends_with(".weba")) result = "audio/webm";
    if (view.ends_with(".webm")) result = "video/webm";
    if (view.ends_with(".webp")) result = "image/webp";
    if (view.ends_with(".woff")) result = "font/woff";
    if (view.ends_with(".woff2")) result = "font/woff2";
    if (view.ends_with(".xhtml")) result = "application/xhtml_xml";
    if (view.ends_with(".xls")) result = "application/vnd.ms-excel";
    if (view.ends_with(".xlsx")) result = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    if (view.ends_with(".xml")) result = "application/xml";
    if (view.ends_with(".xul")) result = "application/vnd.mozilla.xul+xml";
    if (view.ends_with(".zip")) result = "application/zip";
    if (view.ends_with(".3gp")) result = "video/3gpp";
    if (view.ends_with(".3g2")) result = "video/3gpp2";
    if (view.ends_with(".7z")) result = "application/x-7z-compressed";

    return true;
}

ultralight::FileHandle let::ultralight_filesystem::OpenFile(const ultralight::String16 &path, bool open_for_writing) {
    ZoneScopedN("filesystem::open_file");
    const auto string = std::string(ultralight::String(path).utf8().data());

    auto mapped_file = daw::filesystem::memory_mapped_file_t<uint8_t>(string);

    _files[_files.size()] = loaded_file{
            .file = std::move(mapped_file),
            .path = string};

    return _files.size() - 1;
}

int64_t let::ultralight_filesystem::ReadFromFile(ultralight::FileHandle handle, char *data, int64_t length) {
    ZoneScopedN("filesystem::read_file");
    if (const auto file = _files.find(handle); file != _files.end())
    {
        std::memcpy(data, file->second.file.data(), length);
        return length;
    }

    return -1;
}

void let::ultralight_filesystem::CloseFile(ultralight::FileHandle &handle) {
ZoneScopedN("filesystem::close_file");
    if (auto file = _files.find(handle); file != _files.end())
        _files.erase(file);
}
