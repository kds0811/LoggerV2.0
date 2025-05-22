#pragma once
#include <Windows.h>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdio>
#include <memory_resource>
#include <mutex>
#include <string>

namespace Log
{
  class Logger final
  {
    std::array<std::byte, 10240> Raw{};
    std::pmr::monotonic_buffer_resource BufferResource;
    std::pmr::vector<std::pmr::string> StringsVec;
    std::mutex Mutex;

  public:
    static Logger& Get()
    {
      static Logger instance;
      return instance;
    }

    template <typename... Types>
    void Log(Types&&... args)
    {
      std::lock_guard lg{Mutex};
      Logging(std::forward<Types>(args)...);
    }

  private:
    Logger() : BufferResource{Raw.data(), Raw.size(), std::pmr::get_default_resource()}, StringsVec{&BufferResource} {}
    Logger(const Logger&)             = delete;
    Logger(const Logger&&)            = delete;
    Logger& operator=(const Logger&)  = delete;
    Logger& operator=(const Logger&&) = delete;

    template <typename T>
    void Logging(T&& arg)
    {
      Logs(std::forward<T>(arg));
      PrintLog();
      StringsVec.clear();
    }

    template <typename T, typename... Types>
    void Logging(T&& arg, Types&&... args)
    {
      Logs(std::forward<T>(arg));
      Logging(std::forward<Types>(args)...);
    }

    inline void PrintLog()
    {
      for (const auto& s : StringsVec)
      {
        OutputDebugString(s.c_str());
      }
      OutputDebugString("\n");
    }


    inline void Logs(const char* arg)
    {
      if (!arg)
      {
        StringsVec.emplace_back(" Is nullptr");
        return;
      }
      StringsVec.emplace_back(arg);
    }

    template <typename T>
      requires std::is_floating_point_v<T> || std::is_integral_v<T>
    void Logs(T arg)
    {
      static constexpr size_t BufferSize = 64;
      char thread_local buffer[BufferSize];

      if constexpr (std::is_integral_v<T>)
      {
        snprintf(buffer, BufferSize, "%d", static_cast<int>(arg));
      }
      else if constexpr (std::is_floating_point_v<T>)
      {
        snprintf(buffer, BufferSize, "%.6f", static_cast<double>(arg));
      }

      StringsVec.emplace_back(buffer);
    }

    template <typename T>
      requires std::is_pointer_v<T> && (!std::is_same_v<T, const char*>)
    void Logs(T arg)
    {
      static constexpr size_t BufferSize = 64;
      char thread_local buffer[BufferSize];

      if (arg)
      {
        snprintf(buffer, BufferSize, " Address 0x%zx", reinterpret_cast<uintptr_t>(arg));
        StringsVec.emplace_back(buffer);
      }
      else
      {
        StringsVec.emplace_back(" is nullptr ");
      }
    }

    template <typename T>
      requires std::is_same_v<std::decay_t<T>, std::nullptr_t>
    void Logs(T arg)
    {
      StringsVec.emplace_back(" is nullptr ");
    }

    inline void Logs(std::string&& arg) { StringsVec.emplace_back(arg.c_str()); }

    inline void Logs(std::string& arg) { StringsVec.emplace_back(arg.c_str()); }

    inline void Logs() { StringsVec.emplace_back(" "); }

    template <>
    void Logs<bool>(bool arg)
    {
      arg ? StringsVec.emplace_back(" TRUE ") : StringsVec.emplace_back(" FALSE ");
    }

    template <>
    void Logs<const wchar_t*>(const wchar_t* arg)
    {
      static constexpr size_t BufferSize = 256;
      char thread_local buffer[BufferSize];

      if (!arg)
      {
        StringsVec.emplace_back("Is nullptr");
      }

      int size = WideCharToMultiByte(CP_UTF8, 0, arg, -1, nullptr, 0, nullptr, nullptr);
      if (size == 0 || size > static_cast<int>(BufferSize))
      {
        StringsVec.emplace_back(" Conversion error or too long ");
      }
      WideCharToMultiByte(CP_UTF8, 0, arg, -1, buffer, size, nullptr, nullptr);

      StringsVec.emplace_back(buffer);
    }

    inline void Logs(std::wstring& arg) { Logs(arg.c_str()); }

    template <>
    void Logs<wchar_t>(wchar_t arg)
    {
      Logs(&arg);
    }

    template <>
    void Logs<void*>(void* arg)
    {
      StringsVec.emplace_back(" IS VOID PTR ");
    }

    template <typename Key, typename Value>
    void Logs(const std::pair<const Key, Value>& arg)
    {
      Logs(arg.first);
      Logs(": ");
      Logs(arg.second);
    }
  };

} // Logger namespace


#define LOG_WARNING(...) Log::Logger::Get().Log("warning ", "FILE: ", __FILE__, " LINE: ", __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) Log::Logger::Get().Log("error ", "FILE: ", __FILE__, " LINE: ", __LINE__, __VA_ARGS__)
#define LOG_MESSAGE(...) Log::Logger::Get().Log("information ", "FILE: ", __FILE__, " LINE: ", __LINE__, __VA_ARGS__)
