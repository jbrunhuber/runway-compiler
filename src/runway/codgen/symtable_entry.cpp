#include <codegen/symtable_entry.hpp>

/**
 * Release memory for llvm_ptr
 */
SymtableEntry::~SymtableEntry() {

  delete llvm_ptr;
}
