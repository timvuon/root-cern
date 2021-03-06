//--------------------------------------------------------------------*- C++ -*-
// CLING - the C++ LLVM-based InterpreterG :)
// version: $Id$
// author:  Axel Naumann <axel@cern.ch>
//------------------------------------------------------------------------------

#ifndef CLING_STOREDVALUEREF_H
#define CLING_STOREDVALUEREF_H

#include "cling/Interpreter/Value.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

namespace cling {
  ///\brief A type, value pair.
  //
  /// Reference counted wrapper around a cling::Value with storage allocation.
  class StoredValueRef {
  private:
    class StoredValue: public Value, public llvm::RefCountedBase<StoredValue> {
    public:
      /// \brief Construct a valid StoredValue, allocating as needed.
      StoredValue(const clang::ASTContext&, clang::QualType clangTy, 
                  const llvm::Type* llvmTy);
      /// \brief Destruct and deallocate if necessary.
      ~StoredValue();

      /// \brief Number of bytes that need to be allocated to hold a value
      /// of our type
      long long getAllocSizeInBytes(const clang::ASTContext& ctx) const;

      /// \brief Memory allocated for value, owned by this value
      ///
      /// Points to memory allocated for storing the value, if it
      /// does not fit into Value::value.
      char* m_Mem;

      /// \brief Pre-allocated buffer for value
      ///
      /// Can be pointed-to by m_Mem to avoid extra memory allocation for
      /// small values.
      char m_Buf[80]; // increases sizeof(*this) from 48->128
    };

    llvm::IntrusiveRefCntPtr<StoredValue> m_Value;

    StoredValueRef(StoredValue* value): m_Value(value) {}

  public:
    /// \brief Allocate an object of type t and return a StoredValueRef to it.
    static StoredValueRef allocate(const clang::ASTContext& ctx,
                                   clang::QualType t, const llvm::Type* llvmTy);
    /// \brief Create a bitwise copy of value wrapped in a StoredValueRef.
    static StoredValueRef bitwiseCopy(const clang::ASTContext& ctx,
                                      const Value& value);
    /// \brief Create a bitwise copy of svalue.
    static StoredValueRef bitwiseCopy(const clang::ASTContext& ctx,
                                      const StoredValueRef svalue) {
      return bitwiseCopy(ctx, *svalue.m_Value);
    }

    /// \brief Creates an invalid value.
    static StoredValueRef invalidValue() { return StoredValueRef(); }

    /// \brief Construct an empty, invalid value.
    StoredValueRef() {}

    /// \brief Determine whether a Value is available.
    bool isValid() const { return m_Value; }

    /// \brief Determine whether the Value needs to manage an allocation.
    bool needsManagedAllocation() const { return m_Value->m_Mem; }

    const Value& get() const { return *m_Value; }
    Value& get() { return *m_Value; }

    /// \brief Dump the referenced value.
    void dump(clang::ASTContext& ctx) const;
  };
} // end namespace cling

#endif // CLING_STOREDVALUEREF_H
