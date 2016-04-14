#ifndef MULTIVERSO_MATRIX_TABLE_H_
#define MULTIVERSO_MATRIX_TABLE_H_

#include "multiverso/multiverso.h"
#include "multiverso/table_interface.h"
#include "multiverso/util/log.h"

#include <vector>

namespace multiverso {

template <typename T>
class MatrixWorkerTable : public WorkerTable {
public:
  MatrixWorkerTable(int num_row, int num_col);

  // get whole table, data is user-allocated memory
  void Get(T* data, size_t size);

  // data is user-allocated memory
  void Get(int row_id, T* data, size_t size);

  void Get(const std::vector<int>& row_ids,
           const std::vector<T*>& data_vec, size_t size);

  // Add whole table
  void Add(T* data, size_t size, const UpdateOption* option = nullptr);

  void Add(int row_id, T* data, size_t size, 
           const UpdateOption* option = nullptr);

  void Add(const std::vector<int>& row_ids,
           const std::vector<T*>& data_vec, size_t size, 
           const UpdateOption* option = nullptr);

  int Partition(const std::vector<Blob>& kv,
    std::unordered_map<int, std::vector<Blob>>* out) override;

  void ProcessReplyGet(std::vector<Blob>& reply_data) override;

private:
  std::unordered_map<int, T*> row_index_;  // index of data with row id in data_vec_
  int get_reply_count_;                    // number of unprocessed get reply
  int num_row_;
  int num_col_;
  int row_size_;                           // equals to sizeof(T) * num_col_
  int num_server_;
  std::vector<int> server_offsets_;        // row id offset
};

template <typename T>
class Updater;

template <typename T>
class MatrixServerTable : public ServerTable {
public:
  MatrixServerTable(int num_row, int num_col);

  void ProcessAdd(const std::vector<Blob>& data) override;

  void ProcessGet(const std::vector<Blob>& data,
                  std::vector<Blob>* result) override;

  void Store(Stream* s) override;
  void Load(Stream* s) override;

private:
  int server_id_;
  int my_num_row_;
  int num_col_;
  int row_offset_;
  Updater<T>* updater_;
  std::vector<T> storage_;
};

//older implementation
template <typename T>
class MatrixTableHelper : public TableHelper {
public:
  MatrixTableHelper(int num_row, int num_col) : num_row_(num_row), num_col_(num_col){}
  ~MatrixTableHelper() {}

protected:
  WorkerTable* CreateWorkerTable() override{
    return new MatrixWorkerTable<T>(num_row_, num_col_);
  }
  ServerTable* CreateServerTable() override{
    return new MatrixServerTable<T>(num_row_, num_col_);
  }
  int num_row_;
  int num_col_;
};

////new implementation
//template<typename T>
//class MatrixTableFactory : public TableFactory {
//public:
//  /*
//  * args[0] : num_row
//  * args[1] : num_col
//  */
//  MatrixTableFactory(const std::vector<void*>&args) {
//    CHECK(args.size() == 2);
//    num_row_ = *(int*)args[0];
//    num_col_ = *(int*)args[1];
//  }
//protected:
//  WorkerTable* CreateWorkerTable() override{
//    return new MatrixWorkerTable<T>(num_row_, num_col_);
//  }
//  ServerTable* CreateServerTable() override{
//    return new MatrixServerTable<T>(num_row_, num_col_);
//  }
//  int num_row_;
//  int num_col_;
//};
}

#endif // MULTIVERSO_MATRIX_TABLE_H_
