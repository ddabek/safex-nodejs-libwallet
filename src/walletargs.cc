#include "walletargs.h"

#include <stdexcept>

#include <nan.h>
#include <string>

using namespace v8;

namespace exawallet {

const uint32_t CreateTransactionArgs::DEFAULT_MIXIN = 6;

template<typename T>
T convertValue(Local<Value> value);

bool hasProperty(Local<Context> context, Local<Object> obj, Local<String> name) {
    auto maybe = obj->HasOwnProperty(context, name);
    bool has = false;
    maybe.To(&has);
    return has;
}

template<typename T>
bool getRequiredProperty(Local<Object> obj, const char* name, T& value) {
    Local<Context> context = Nan::GetCurrentContext();

    auto key = Nan::New(name).ToLocalChecked();
    bool has = hasProperty(context, obj, key);
    if (!has){
        return false;
    }

    auto valueLocal = obj->Get(context, key).ToLocalChecked();
    value = convertValue<T>(valueLocal);
    return true;
}

template<typename T>
T getOptionalProperty(Local<Object> obj, const char* name, const T& defaultValue) {
    Local<Context> context = Nan::GetCurrentContext();

    auto key = Nan::New(name).ToLocalChecked();
    bool has = hasProperty(context, obj, key);
    if (!has){
        return defaultValue;
    }

    auto valueLocal = obj->Get(context, key).ToLocalChecked();
    return convertValue<T>(valueLocal);
}

template<>
std::string convertValue<std::string>(Local<Value> value) {
    Nan::Utf8String str(value);
    return *str;
}

template<>
double convertValue<double>(Local<Value> value) {
    return value->ToNumber(Nan::GetCurrentContext()).ToLocalChecked()->Value();
}

template<>
uint32_t convertValue<uint32_t>(Local<Value> value) {
    return value->ToUint32(Nan::GetCurrentContext()).ToLocalChecked()->Value();
}

template<>
uint64_t convertValue<uint64_t>(Local<Value> value) {
    return value->ToUint32(Nan::GetCurrentContext()).ToLocalChecked()->Value();
}

std::string CreateWalletArgs::Init(const Nan::FunctionCallbackInfo<Value>& args) {
    if (args.Length() != 1 || !args[0]->IsObject()) {
        return "Argument must be an object";
    }
    auto obj = Nan::To<v8::Object>(args[0]).ToLocalChecked();
    if (!getRequiredProperty<std::string>(obj, "path", path)) {
        return std::string("Required property not found: path");
    }

    if (!getRequiredProperty<std::string>(obj, "password", password)) {
        return std::string("Required property not found: password");
    }

    if (!getRequiredProperty<std::string>(obj, "daemonAddress", daemonAddress)) {
        return std::string("Required property not found: daemonAddress");
    }

    language = getOptionalProperty<std::string>(obj, "language", "English");
    auto net = getOptionalProperty<std::string>(obj, "network", "mainnet");
    if (net == "mainnet") {
        nettype = Safex::MAINNET;
    } else if (net == "testnet") {
        nettype = Safex::TESTNET;
    } else if (net == "stagenet") {
        nettype = Safex::STAGENET;
    } else {
        return "Invalid value for network: " + net;
    }

    return {};
}

  std::string CreateWalletFromKeysArgs::Init(const Nan::FunctionCallbackInfo<Value>& args) {
      if (args.Length() != 1 || !args[0]->IsObject()) {
          return "Argument must be an object";
      }
      auto obj = Nan::To<v8::Object>(args[0]).ToLocalChecked();
      if (!getRequiredProperty<std::string>(obj, "path", path)) {
          return std::string("Required property not found: path");
      }

      if (!getRequiredProperty<std::string>(obj, "password", password)) {
          return std::string("Required property not found: password");
      }

      if (!getRequiredProperty<std::string>(obj, "daemonAddress", daemonAddress)) {
          return std::string("Required property not found: daemonAddress");
      }

      language = getOptionalProperty<std::string>(obj, "language", "English");
      auto net = getOptionalProperty<std::string>(obj, "network", "mainnet");
      if (net == "mainnet") {
          nettype = Safex::MAINNET;
      } else if (net == "testnet") {
          nettype = Safex::TESTNET;
      } else if (net == "stagenet") {
          nettype = Safex::STAGENET;
      } else {
          return "Invalid value for network: " + net;
      }

      restoreHeight = getOptionalProperty<uint64_t>(obj, "restoreHeight", 0);

      if (!getRequiredProperty<std::string>(obj, "addressString", addressString)) {
          return std::string("Required property not found: addressString");
      }

      if (!getRequiredProperty<std::string>(obj, "viewKeyString", viewKeyString)) {
          return std::string("Required property not found: viewKeyString");
      }

      if (!getRequiredProperty<std::string>(obj, "spendKeyString", spendKeyString)) {
          return std::string("Required property not found: spendKeyString");
      }

      return {};
  }

std::string OpenWalletArgs::Init(const Nan::FunctionCallbackInfo<Value>& args) {
    if (args.Length() != 1 || !args[0]->IsObject()) {
        return "Argument must be an object";
    }

    auto obj = Nan::To<v8::Object>(args[0]).ToLocalChecked();
    if (!getRequiredProperty<std::string>(obj, "path", path)) {
        return std::string("Required property not found: path");
    }
    if (!getRequiredProperty<std::string>(obj, "password", password)) {
        return std::string("Required property not found: password");
    }
    if (!getRequiredProperty<std::string>(obj, "daemonAddress", daemonAddress)) {
        return std::string("Required property not found: daemonAddress");
    }

    auto net = getOptionalProperty<std::string>(obj, "network", "mainnet");
    if (net == "mainnet") {
        nettype = Safex::MAINNET;
    } else if (net == "testnet") {
        nettype = Safex::TESTNET;
    } else if (net == "stagenet") {
        nettype = Safex::STAGENET;
    } else {
        return "Invalid value for network: " + net;
    }

    return {};
}


std::string RecoveryWalletArgs::Init(const Nan::FunctionCallbackInfo<Value>& args) {
    if (args.Length() != 1 || !args[0]->IsObject()) {
        return "Argument must be an object";
    }

    auto obj = Nan::To<v8::Object>(args[0]).ToLocalChecked();
    if (!getRequiredProperty<std::string>(obj, "path", path)) {
        return std::string("Required property not found: path");
    }
    if (!getRequiredProperty<std::string>(obj, "password", password)) {
        return std::string("Required property not found: password");
    }
    if (!getRequiredProperty<std::string>(obj, "daemonAddress", daemonAddress)) {
        return std::string("Required property not found: daemonAddress");
    }

    auto net = getOptionalProperty<std::string>(obj, "network", "mainnet");
    if (net == "mainnet") {
        nettype = Safex::MAINNET;
    } else if (net == "testnet") {
        nettype = Safex::TESTNET;
    } else if (net == "stagenet") {
        nettype = Safex::STAGENET;
    } else {
        return "Invalid value for network: " + net;
    }

    restoreHeight = getOptionalProperty<uint64_t>(obj, "restoreHeight", 0);

    if(!getRequiredProperty<std::string>(obj, "mnemonic", mnemonic)) {
        return std::string("Required property not found: mnemonic");
    }

    return {};
}


std::string CreateTransactionArgs::Init(const Nan::FunctionCallbackInfo<Value>& args) {
    if (args.Length() != 1 || !args[0]->IsObject()) {
        return "Argument must be an object";
    }

    auto obj = Nan::To<v8::Object>(args[0]).ToLocalChecked();

    if (!getRequiredProperty<std::string>(obj, "address", address)) {
        return std::string("Required property not found: address");
    }

    std::string amountStr;
    if (!getRequiredProperty<std::string>(obj, "amount", amountStr)) {
        return std::string("Required property not found: amount");
    }
    amount = std::stoull(amountStr);
    paymentId = getOptionalProperty<std::string>(obj, "paymentId", "");
    mixin = getOptionalProperty<uint32_t>(obj, "mixin", DEFAULT_MIXIN);
    priority = static_cast<Safex::PendingTransaction::Priority>(getOptionalProperty<uint32_t>(obj, "priority",
            static_cast<uint32_t>(Safex::PendingTransaction::Priority::Priority_Medium)));
    tx_type = static_cast<Safex::TransactionType>(getOptionalProperty<uint32_t>(obj, "tx_type",
            static_cast<uint32_t>(Safex::TransactionType::CashTransaction)));

    return {};
}

}
