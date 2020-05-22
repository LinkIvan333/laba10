// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_MAIN_HPP_
#define INCLUDE_MAIN_HPP_
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/unordered_map.hpp>
#include <rocksdb/db.h>
#include <rocksdb/slice.h>
#include <rocksdb/options.h>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include <mutex>
#include <vector>
#include <list>
#include <picosha2.hpp>
#include <constants.hpp>

using FContainer =
        std::list<std::unique_ptr<rocksdb::ColumnFamilyHandle>>;
using FDescriptorContainer =
        std::vector<rocksdb::ColumnFamilyDescriptor>;
using FHandlerContainer =
        std::list<std::unique_ptr<rocksdb::ColumnFamilyHandle>>;
using StrContainer = boost::unordered_map<std::string, std::string>;
namespace po = boost::program_options;

const std::size_t DEFAULT_THREAD_HASH = boost::thread::hardware_concurrency();
const std::string DEFAULT_LOG_LEVEL = "error";
const std::string DEFAULT_OUTPUT_PATH = "/home/ivan/DB/new_db";
const std::string DEFAULT_INPUT_PATH = "/home/ivan/DB/db";

class DBHashCreator {
public:
    explicit DBHashCreator(std::string path) : _path(path) {
       new_options.create_if_missing = true;
       options.create_if_missing = true;
    }

    DBHashCreator(std::string path,
            std::size_t threadCount,
            std::string logLVL) :
            _path(path),
            _logLVL(logLVL),  _threadCountHash(threadCount){
      new_options.create_if_missing = true;
      options.create_if_missing = true;
    }

    FDescriptorContainer getFamilyDescriptors();

    FHandlerContainer openDB(const FDescriptorContainer &);

    StrContainer getStrs(rocksdb::ColumnFamilyHandle *);

    void getHash(rocksdb::ColumnFamilyHandle *, StrContainer);

    void startHash(FHandlerContainer *, std::list<StrContainer> *);

    void startThreads();

private:
    rocksdb::Options new_options;
    rocksdb::DBOptions options;
    std::string _new_path = DEFAULT_OUTPUT_PATH;
    std::string _path = DEFAULT_INPUT_PATH;
    std::string _logLVL = DEFAULT_LOG_LEVEL;
    std::unique_ptr<rocksdb::DB> _db, _new_db;
    std::size_t _threadCountHash = DEFAULT_THREAD_HASH;
    std::mutex _mutex;
};
#endif // INCLUDE_MAIN_HPP_
