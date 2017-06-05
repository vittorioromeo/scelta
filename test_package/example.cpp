#include "scelta.hpp"

int main(){
  
  using shape = std::variant<circle, box>;

  shape s0{circle{/*...*/}};
  shape s1{box{/*...*/}};

  // In place `match` visitation.
  scelta::match([](circle, circle){ /* ... */ },
                [](circle, box)   { /* ... */ },
                [](box,    circle){ /* ... */ },
                [](box,    box)   { /* ... */ })(s0, s1);
}