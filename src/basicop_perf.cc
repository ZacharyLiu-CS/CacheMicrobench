//
//  single_thread_ops.cc
//  PROJECT single_thread_ops
//
//  Created by zhenliu on 22/06/2022.
//  Copyright (c) 2022 zhenliu <liuzhenm@mail.ustc.edu.cn>.
//

#include <bits/stdint-intn.h>
#include <fcntl.h>
#include <gflags/gflags.h>
#include <gflags/gflags_declare.h>
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <thread>

#include "include/histogram.h"
#include "include/logging.h"
#include "include/memory_ops.h"
#include "include/numa_config.h"
#include "include/pmem_ops.h"
#include "include/profiler.h"
#include "include/sequential_generator.h"
#include "include/statistics.h"
#include "include/uniform_generator.h"
#include "include/zipfian_generator.h"

// defined in logging.h
// DEFINE_bool(debug, false, "Set if output log message");
DEFINE_uint64(pmem_size, 20 * 1024UL * 1024UL * 1024UL,
              "The size of test pmem space size. (default: 20GB)");
DEFINE_int32(io_size, 64, "The size of single opeartion. (default: 64B)");
DEFINE_string(pmem_path, "/mnt/pmem0/device_perf/basicop_perf.map",
              "Path of pmem device.");
DEFINE_string(pmem_mode, "fsdax", "Mode of device usage. Option: fsdax/devdax");
DEFINE_int32(thread_num, 4, "Test thread of operation");
DEFINE_int32(op_per_thread, 100000, "Test num of operations");
DEFINE_string(operation_type, "all", "Option: all/read/write");
DEFINE_int32(workload_type, 0,
             "Types of workload: 0 -> zipfian(default) / 1 -> sequential / 2 -> uniform");
DEFINE_int32(bind_cpu, 0,
             "CPU socket id of the thread want to bind: 0 (default)");

// define the workload type
enum WorkloadTypeList : uint32_t {
  zipfian = 0,
  sequential = 1,
  uniform = 2,
  type_max
};

//
using WorkloadType = int32_t;

// define the pmem operation test parameters
// @1 start address
// @2 test space size
// @3 workload type
struct Pmem_thread_params {
  char *start_addr;
  uint64_t test_space;
  uint32_t cpu_id;
  WorkloadType wtype;
};

void TestWriteBody(Pmem_thread_params param, utils::Histogram *histogram) {
  // LOG("\n", (void*)param.start_addr, param.test_space, param.cpu_id,
  // param.wtype);
  uint32_t io_size = FLAGS_io_size;
  char src_data[io_size];
  memset(src_data, 'I', io_size);

  // inilization generator
  Generator<std::uint64_t> *generator = nullptr;
  uint32_t space_unit_size = io_size;
  switch (param.wtype) {
    case WorkloadTypeList::sequential:
      space_unit_size = io_size;
      generator =
          new SequentialGenerator(0, param.test_space / space_unit_size);
      break;
    case WorkloadTypeList::uniform:
      generator = new UniformGenerator(0, param.test_space / space_unit_size);
      break;
    case WorkloadTypeList::zipfian:
      generator = new UniformGenerator(0, param.test_space / space_unit_size);
      break;
  }

  PointPorfiler pperf;
  for (int i = 0; i < FLAGS_op_per_thread; i++) {
    char *temp_addr = param.start_addr + space_unit_size * generator->Next();
    // record start
    pperf.start();

    // Test main body
    memcpy(temp_addr, src_data, io_size);
    clflush(temp_addr, io_size);

    pperf.end();

    // record end
    histogram->Add_Fast(pperf.duration());
  }
  delete generator;
}

void TestReadBody(Pmem_thread_params param, utils::Histogram *histogram) {
  // LOG("\n", (void*)param.start_addr, param.test_space, param.cpu_id,
  // param.wtype);
  uint32_t io_size = FLAGS_io_size;
  char dst_data[io_size];
  memset(dst_data, 'J', io_size);

  // inilization generator
  Generator<std::uint64_t> *generator = nullptr;
  uint32_t space_unit_size = io_size;
  switch (param.wtype) {
    case WorkloadTypeList::sequential:
      space_unit_size = io_size;
      generator =
          new SequentialGenerator(0, param.test_space / space_unit_size);
      break;
    case WorkloadTypeList::uniform:
      generator = new UniformGenerator(0, param.test_space / space_unit_size);
      break;
    case WorkloadTypeList::zipfian:
      generator = new UniformGenerator(0, param.test_space / space_unit_size);
      break;
  }

  PointPorfiler pperf;
  for (int i = 0; i < FLAGS_op_per_thread; i++) {
    char *temp_addr = param.start_addr + space_unit_size * generator->Next();
    // record start
    pperf.start();

    // Test main body
    memcpy(dst_data, temp_addr, io_size);
    mfence();

    pperf.end();
    // record end
    histogram->Add_Fast(pperf.duration());
  }
  delete generator;
}

// set thread affinity to cpu core
inline void bind_thread_tocpu(pthread_t t, int32_t cpuid) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpuid, &cpuset);
  int rc = pthread_setaffinity_np(t, sizeof(cpuset), &cpuset);
  if (rc != 0) {
    LOG("Error calling pthread_setaffinity_np", rc);
  }
}

int main(int argc, char **argv) {
  gflags::SetUsageMessage("Usage message of pmem operation test:");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  LOG("\n--------------------configuration output-----------------");
  LOG("pmem mode:", FLAGS_pmem_mode);
  LOG("pmem path:", FLAGS_pmem_path);
  LOG("pmem size:", FLAGS_pmem_size);
  LOG("workload type:", PmemTickersNameMap[FLAGS_workload_type].second, FLAGS_operation_type);
  LOG("operation per num:", FLAGS_op_per_thread);
  LOG("operation type:", FLAGS_operation_type);
  LOG("bind cpu:", FLAGS_bind_cpu);
  LOG("thread num:", FLAGS_thread_num);
  LOG("------------------------end output-----------------------\n");
  // map the pmem space
  // size = 5 GB
  const char *pmem_dev = FLAGS_pmem_path.c_str();
  const uint64_t pmem_size = FLAGS_pmem_size;
  int fd = open(pmem_dev, O_RDWR | O_CREAT | O_TRUNC, 0666);
  assert(fd > 0);
  if (FLAGS_pmem_mode == "fsdax") {
    int fres = ftruncate(fd, pmem_size);
    if (fres != 0) {
      LOG("Error ftruncate pmem space!");
      exit(0);
    }
  }
  char *test_space =
      (char *)mmap(0, pmem_size, PROT_READ | PROT_WRITE, 0x8003, fd, 0);
  assert(test_space);

  // each thread do write/read job ,then record it to their own histogram
  std::vector<std::thread> threads(FLAGS_thread_num);

  // control if run the write test body
  if (FLAGS_operation_type == "all" || FLAGS_operation_type == "write") {
    std::vector<utils::Histogram> wlist(FLAGS_thread_num);
    for (int i = 0; i < FLAGS_thread_num; i++) {
      Pmem_thread_params p1 = {
          .start_addr = RTE_PTR_ALIGN(
              test_space + i * (FLAGS_pmem_size / FLAGS_thread_num), 16),
          .test_space = FLAGS_pmem_size / FLAGS_thread_num,
          .cpu_id = OS_CPU_ID[FLAGS_bind_cpu][i][0],
          .wtype = FLAGS_workload_type,
      };
      threads[i] = std::thread(TestWriteBody, p1, &wlist[i]);
      bind_thread_tocpu(threads[i].native_handle(), p1.cpu_id);
    }

    for (auto &t : threads) {
      t.join();
    }

    utils::Histogram merge_write_histogram;
    for (const auto &h : wlist) {
      merge_write_histogram.Merge(h);
    }
    LOG("Workload: ", PmemTickersNameMap[FLAGS_workload_type].second, FLAGS_operation_type,
        "\nOPS : ", calculate_ops(FLAGS_thread_num, merge_write_histogram),
        "\nThpt: ",
        calculate_thpt(FLAGS_io_size, FLAGS_thread_num, merge_write_histogram) /
            SizeUnit::h_gigabytes,
        "GiB/s",
        "\nWrite Latency Distribution: ", merge_write_histogram.ToString());
  }
  //------------------------------------------------------
  // Now here is read process
  if (FLAGS_operation_type == "all" || FLAGS_operation_type == "read") {
    std::vector<utils::Histogram> rlist(FLAGS_thread_num);
    for (int i = 0; i < FLAGS_thread_num; i++) {
      Pmem_thread_params p1 = {
          .start_addr = RTE_PTR_ALIGN(
              test_space + i * (FLAGS_pmem_size / FLAGS_thread_num), 16),
          .test_space = FLAGS_pmem_size / FLAGS_thread_num,
          .cpu_id = OS_CPU_ID[FLAGS_bind_cpu][i][0],
          .wtype = FLAGS_workload_type,
      };
      threads[i] = std::thread(TestReadBody, p1, &rlist[i]);
      bind_thread_tocpu(threads[i].native_handle(), p1.cpu_id);
    }

    for (auto &t : threads) {
      t.join();
    }

    utils::Histogram merge_read_histogram;
    for (const auto &h : rlist) {
      merge_read_histogram.Merge(h);
    }
    LOG("Workload: ", PmemTickersNameMap[FLAGS_workload_type].second, FLAGS_operation_type,
        "\nOPS : ", calculate_ops(FLAGS_thread_num, merge_read_histogram),
        "\nThpt: ",
        calculate_thpt(FLAGS_io_size, FLAGS_thread_num, merge_read_histogram) /
            SizeUnit::h_gigabytes,
        "GiB/s",
        "\nRead Latency Distribution: ", merge_read_histogram.ToString());
  }
  // create test thread and pin them to physical cores
  // pmem is on core 0, so we need to pin thread to core 0
  // char* test_space = (char*)mmap(0, pmem_size, PROT_READ | PROT_WRITE,
  // 0x8003, fd, 0);
  munmap(test_space, pmem_size);
  close(fd);
  google::ShutDownCommandLineFlags();
  return 0;
}
