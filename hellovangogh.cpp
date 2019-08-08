#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <cmath>
#include <eosio/system.hpp> 
#include <eosio/time.hpp>
#include <eosio/singleton.hpp>

using namespace eosio;

class [[eosio::contract("hellovangogh")]] hellovangogh : public eosio::contract {

public:
  using contract::contract;
  
  hellovangogh(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  // void apply(account_name contract,action_name act);
  
  float computeratio(asset investbalanc){
    auto amount = investbalanc.amount;
    if (amount >= 900000000000){
      return 0.3414;
    }else if(amount >= 800000000000){
      return 0.3793;
    }else if(amount >= 700000000000){
      return 0.4268;
    }else if(amount >= 600000000000){
      return 0.4877;
    }else if(amount >= 500000000000){
      return 0.569;
    }else if(amount >= 400000000000){
      return 0.6828;
    }else if(amount >= 300000000000){
      return 0.8535;
    }else if(amount >= 200000000000){
      return 1.138;
    }else if(amount >= 100000000000){
      return 1.707;
    }else{
      return 3.414;
    }
  };
  
  float computenewratio(float ratio){
    if(ratio >= 3.414){
      return 1.707;
    }else if(ratio >= 1.707){
      return 1.138;
    }else if(ratio >= 1.138){
      return 0.8535;
    }else if(ratio >= 0.8535){
      return 0.6828;
    }else if(ratio >= 0.6828){
      return 0.569;
    }else if(ratio >= 0.569){
      return 0.4877;
    }else if(ratio >= 0.4877){
      return 0.4268;
    }else if(ratio >= 0.3793){
      return 0.3414;
    }else{
      return 0.3414;
    }
  };
  
  int computelevel(asset investbalanc){
    auto amount = investbalanc.amount;
    return floor(amount / 100000000000)+1;
  };
  
  uint64_t computeinvestetoken(asset investedtoken,float ratio,int level,asset quantity){
    auto allinvested = quantity.amount * ratio + investedtoken.amount;
    int newlevel = floor(allinvested / 100000000000)+1;
    if (newlevel>level){
      int lastleveltoken = (int) ((level*100000000000 - investedtoken.amount) / ratio);
      float newratio = computenewratio(ratio);
      float newleveltoken = ((quantity.amount - lastleveltoken) * newratio);
      return (level*100000000000 - investedtoken.amount) + newleveltoken;
    }else{
      return quantity.amount*ratio;
    }
  };
  
  [[eosio::action]]
  void transfer(name from,name to,asset quantity,std::string memo){
    check(from != to,"cannot transfer to self");
    require_auth(from);
    check(is_account(to),"to account does not exists");
    // auto sym = quantity.symbol.name;
    // name name_ = string_to_name("ipsecontract");
    symbol symbol_ = symbol("POST", 4);
    name to_ = name("hellovangogh");
    check(to == to_,"must transfer to the hellovangogh contract");
    
    require_recipient(from);
    require_recipient(to);
    check(quantity.is_valid(),"invalid quantity");
    check(quantity.amount >= 1000000000,"must transfer quantity over 100000");
    check(quantity.amount <= 20000000000,"must transfer quantity not over 2000000 once");
    // check(quantity.symbol.name == name_,"the token is not valid");
    check(quantity.symbol == symbol_,"symbol precision mismatch");
    check(memo.size() <= 256,"memo hash more than 256 bytes");
    
    float ratio = 3.414;
    int level = 1;
    auto investedtoken = quantity;
    globalvar_index globalvar(get_first_receiver(),to_.value);
    auto globalvaritr = globalvar.find(0);
    if(globalvaritr != globalvar.end()){
      asset investbalanc = globalvaritr -> investbalanc;
      ratio = computeratio(investbalanc);
      level = computelevel(investbalanc);
      auto allbalance = investbalanc.amount + quantity.amount*ratio;
      investedtoken = investbalanc;
      check(allbalance <= 1000000000000,"the investbalance over 100000000");
    }
    
    // if someone transfer POSC to hellovangogh,then process investing 
    invest_index investtable(get_first_receiver(),to_.value);
    auto investetoken = computeinvestetoken(investedtoken,ratio,level,quantity);
    asset shouldinvest = asset(investetoken,symbol_);
    auto itr = investtable.find(from.value);
    if(itr != investtable.end()){
      asset investtoken = itr -> investtoken;
      auto allinvest = investtoken.amount + shouldinvest.amount;
      asset all_invest = asset(allinvest,symbol_);
      uint32_t createtime = current_time_point().sec_since_epoch();
      investtable.modify(itr,get_first_receiver(), [&]( auto& row ) {
          row.investtoken = all_invest;
          row.createtime = createtime;
          row.memo = memo;
      });
      print("invest success again");
    }else{
      investtable.emplace(get_first_receiver(), [&]( auto& row){
        row.address = from;
        row.investtoken = shouldinvest;
        row.createtime = current_time_point().sec_since_epoch();
        row.status = "invested";
        row.memo = memo;
      });
      print("first invest success");
    }
    
    if(globalvaritr != globalvar.end()){
      asset investbalanc = globalvaritr -> investbalanc;
      asset totaltoken = globalvaritr -> totaltoken;
      auto allbalance = investbalanc.amount + shouldinvest.amount;
      auto alltoken = totaltoken.amount + shouldinvest.amount;
      asset all_balance = asset(allbalance,symbol_);
      asset all_token = asset(alltoken,symbol_);
      globalvar.modify(globalvaritr,get_first_receiver(), [&]( auto& row ){
        row.investbalanc = all_balance;
        row.totaltoken = all_token;
      });
    }else{
      globalvar.emplace(get_first_receiver(), [&]( auto& row){
        row.id = 0;
        row.address = to_;
        row.investbalanc = shouldinvest;
        row.totaltoken = shouldinvest;
        row.status = "begining";
      });
    }
  };
  
  [[eosio::action]]
  void quitinvest(name from,asset quantity){
    require_auth(from);
    symbol symbol_ = symbol("POST", 4);
    name to_ = name("hellovangogh");
    check(quantity.is_valid(),"invalid quantity");
    check(quantity.amount > 0,"must postive token quit investing");
    invest_index investtable(get_first_receiver(),to_.value);
    auto itr = investtable.find(from.value);
    if(itr != investtable.end()){
      asset investtoken = itr -> investtoken;
      auto balancetoken = investtoken.amount - quantity.amount * 3.414;
      if(balancetoken>=0){
        investtable.modify(itr,get_first_receiver(), [&]( auto& row ){
          row.investtoken = asset(balancetoken,symbol_);
        });
        globalvar_index globalvar(get_first_receiver(),to_.value);
        auto globalvaritr = globalvar.find(0);
        if(globalvaritr != globalvar.end()){
          asset investbalanc = globalvaritr -> investbalanc;
          asset transfertoke = globalvaritr -> transfertoke;
          asset investbalanc_ = asset(investbalanc.amount - quantity.amount * 3.414,symbol_);
          asset transfertoke_ = asset(transfertoke.amount + quantity.amount * 3.414,symbol_);
          globalvar.modify(globalvaritr,get_first_receiver(),[&]( auto& row ){
            row.investbalanc = investbalanc_;
            row.transfertoke = transfertoke_;
          });
        }
        // SEND_INLINE_ACTION
        action(
          permission_level{_self,"active"_n} ,
          "ipsecontract"_n,"transfer"_n,
          std::make_tuple(_self,from,quantity,std::string("quit investing"))
        ).send();
      }else{
       print("you withdraw over the balance."); 
      }
    }else{
      print("you did not invested!");
    }
  };
  
  [[eosio::action]]
  void selltoken(name to,asset quantity,std::string memo){
    name to_ = name("hellovangogh");
    symbol symbol_ = symbol("POST", 4);
    require_auth(to_);
    globalvar_index globalvar(get_first_receiver(),to_.value);
    auto globalvaritr = globalvar.find(0);
    if(globalvaritr != globalvar.end()){
      asset investbalanc = globalvaritr -> investbalanc;
      asset transfertoke = globalvaritr -> transfertoke;
      if(quantity.amount <= investbalanc.amount){
        globalvar.modify(globalvaritr,get_first_receiver(),[&]( auto& row){
          row.investbalanc = asset(investbalanc.amount - quantity.amount,symbol_);
          row.transfertoke = asset(transfertoke.amount + quantity.amount,symbol_);
        });
        action(
          permission_level{_self,"active"_n} ,
          "ipsecontract"_n,"transfer"_n,
          std::make_tuple(_self,to,quantity,memo)
        ).send();
      }else{
        print("need more investing");
      }
    }else{
      print("no investing now");
    }
  };
  
private:
  struct [[eosio::table]] investtable {
    name  address;
    asset investtoken;
    uint32_t  createtime;
    std::string status;
    std::string  memo;
    uint64_t primary_key() const { return address.value; }
    EOSLIB_SERIALIZE(investtable,(address)(investtoken)(createtime)(status)(memo))
  };
  typedef eosio::multi_index<"investtable"_n,investtable> invest_index;

  struct [[eosio::table]] globalvar {
    uint64_t id;
    name address;
    asset investbalanc;
    asset totaltoken;
    asset transfertoke;
    std::string status;
    uint64_t primary_key() const { return id; }
    uint64_t get_secondary_1() const { return address.value; }
    EOSLIB_SERIALIZE(globalvar,(id)(address)(investbalanc)(totaltoken)(transfertoke)(status))
  };
  typedef eosio::multi_index<"globalvar"_n,globalvar,indexed_by<"byaddress"_n,const_mem_fun<globalvar,uint64_t,&globalvar::get_secondary_1>>> globalvar_index;
};   

extern "C" {
  void apply(uint64_t receiver,uint64_t code,uint64_t action){
    check(receiver == "hellovangogh"_n.value,"reject");
    hellovangogh thiscontract(receiver);
    if((code=="ipsecontract"_n.value) && (action=="transfer"_n.value)){
        execute_action(hellovangogh,&thiscontract::transfer);
        return;
    }
    if(code!=receiver) return;
    switch (action) {EOSIO_API(&hellovangogh,(quitinvest)(selltoken))};
    eosio_exit(0);        
  }
}