# scelta
WIP

---

* C++17 introduces variant/optional, will hopefully become common use + vocabulary type

* Visitation is often more verbose than required

* Code bases might use multiple variant/optional types depending on their semantics, common interface for visitation is desirable

* Works with your variant types

* Various patterns: `match`, `match_recursively`, recursive `struct` visitor + `match`, `y_combinator` + `match`, etc.

* TODO: bldb variant
