#include "multiverso/table_interface.h"
#include "multiverso/util/log.h"
#include "multiverso/util/waiter.h"
#include "multiverso/zoo.h"
#include "multiverso/dashboard.h"
#include "multiverso/updater/updater.h"

namespace multiverso {

WorkerTable::WorkerTable() {
  table_id_ = Zoo::Get()->RegisterTable(this);
}

ServerTable::ServerTable() {
  Zoo::Get()->RegisterTable(this);
}

void WorkerTable::Get(Blob keys) {
  MONITOR_BEGIN(WORKER_TABLE_SYNC_GET)
  Wait(GetAsync(keys));
  MONITOR_END(WORKER_TABLE_SYNC_GET)
}

void WorkerTable::Add(Blob keys, Blob values,
                      const UpdateOption* option) {
  MONITOR_BEGIN(WORKER_TABLE_SYNC_ADD)
  // Wait(AddAsync(keys, values));
  AddAsync(keys, values, option);
  MONITOR_END(WORKER_TABLE_SYNC_ADD)
}

int WorkerTable::GetAsync(Blob keys) {
  int id = msg_id_++;
  waitings_[id] = new Waiter();
  MessagePtr msg(new Message());
  msg->set_src(Zoo::Get()->rank());
  msg->set_type(MsgType::Request_Get);
  msg->set_msg_id(id);
  msg->set_table_id(table_id_);
  msg->Push(keys);
  Zoo::Get()->SendTo(actor::kWorker, msg);
  return id;
}

int WorkerTable::AddAsync(Blob keys, Blob values,
                          const UpdateOption* option) {
  int id = msg_id_++;
  waitings_[id] = new Waiter();
  MessagePtr msg(new Message());
  msg->set_src(Zoo::Get()->rank());
  msg->set_type(MsgType::Request_Add);
  msg->set_msg_id(id);
  msg->set_table_id(table_id_);
  msg->Push(keys);
  msg->Push(values);
  // Add update option if necessary
  if (option != nullptr) {
    Blob update_option(option->data(), option->size());
    msg->Push(update_option);
  }
  Zoo::Get()->SendTo(actor::kWorker, msg);
  return id;
}

void WorkerTable::Wait(int id) {
  CHECK(waitings_.find(id) != waitings_.end());
  CHECK(waitings_[id] != nullptr);
  waitings_[id]->Wait();
  delete waitings_[id];
  waitings_[id] = nullptr;
}

void WorkerTable::Reset(int msg_id, int num_wait) {
  CHECK_NOTNULL(waitings_[msg_id]);
  waitings_[msg_id]->Reset(num_wait);
}

void WorkerTable::Notify(int id) {
  CHECK_NOTNULL(waitings_[id]);
  waitings_[id]->Notify();
}

WorkerTable* TableHelper::CreateTable() {
  if (Zoo::Get()->server_rank() >= 0) {
    CreateServerTable();
  }
  if (Zoo::Get()->worker_rank() >= 0) {
    return CreateWorkerTable();
  }
  return nullptr;
}

}  // namespace multiverso
