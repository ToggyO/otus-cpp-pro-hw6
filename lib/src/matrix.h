#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "traits.h"
#include "utils.h"

/**
 * @deprecated
 *
 * @brief Legacy code base
 */
template <typename TValue, std::size_t DIM_SIZE, TValue DEFAULT_VALUE>
class MatrixLegacy final
{
private:
    using KeyType = std::array<TValue, DIM_SIZE>;
    using MatrixType = MatrixLegacy<TValue, DIM_SIZE, DEFAULT_VALUE>;

    struct Hash
    {
        std::size_t operator()(const KeyType& indexes) const {
            std::size_t h = 0;
            for (auto e : indexes) {
                h ^= std::hash<TValue>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }

    };

    using Storage = std::unordered_map<KeyType, TValue, Hash>;

public:
    const TValue default_value = DEFAULT_VALUE;

    template <size_t DIMENSION, typename Dummy = void> // error: explicit specialization in non-namespace scope ‘class Matrix<TValue, DIM_SIZE, DEFAULT_VALUE>’
    class Indexer
    {
    public:
        Indexer(KeyType& key, MatrixType& matrix)
            : m_key{key}, m_matrix{matrix} {}

        auto operator[](size_t index)
        {
            m_key[DIM_SIZE - DIMENSION] = index;
            return Indexer<DIMENSION - 1>(m_key, m_matrix);
        }

    private:
        KeyType m_key;
        MatrixType &m_matrix;
    };

    template<typename Dummy> // error: explicit specialization in non-namespace scope ‘class Matrix<TValue, DIM_SIZE, DEFAULT_VALUE>’
    class Indexer<0, Dummy>
    {
    public:
        Indexer(KeyType& key, MatrixType& matrix)
                : m_key{key}, m_matrix{matrix} {}

        Indexer& operator=(const TValue &value)
        {
            m_matrix.m_storage[m_key] = TValue(value);
            return *this;
        }

        Indexer& operator=(TValue &&value)
        {
            m_matrix.m_storage[m_key] = std::move(value);
            return *this;
        }

        operator TValue() const { return m_matrix.get_value(m_key); }

    private:
        KeyType m_key;
        MatrixType& m_matrix;
    };

//    template<typename Dummy> // error: explicit specialization in non-namespace scope ‘class Matrix<TValue, DIM_SIZE, DEFAULT_VALUE>’
//    class Indexer<1, Dummy>
//    {
//    public:
//        Indexer(KeyType& key, MatrixType& matrix)
//                : m_key{key}, m_matrix{matrix} {}
//
//        TValue& operator[](size_t index)
//        {
//            return m_matrix.at(1, 2 , index);
//        }
//
////        Indexer& operator=(const TValue &value)
////        {
////            // TODO: check
//////            apply(matrix, indexes) = std::move(TValue(value));
////            return *this;
////        }
//
//    private:
//        KeyType& m_key;
//        MatrixType& m_matrix;
//    };

    // Ctors
    MatrixLegacy() = default;

    MatrixLegacy(const MatrixType& other)
    {
        m_storage = other.m_storage;
    }

    MatrixLegacy(MatrixType&& other) noexcept
    {
        m_storage = std::move(other.m_storage);
    }

    ~MatrixLegacy() = default;

    // Methods
    template <
        typename... Args,
        typename = enable_if_integral_variadic_t<Args...>
    >
    TValue& at(Args&&... indexes)
    {
        assert(sizeof...(Args) == DIM_SIZE);

        KeyType key;
        fill_array(key, indexes...); // TODO: Narrowing conversion from int to size_t

        return get_value(key);
    }

    template <
        typename... Args,
        typename = enable_if_integral_variadic_t<Args...>
    >
    const TValue& at(Args&&... indexes) const
    {
        return at(std::forward<Args>(indexes)...);
    }

    // Operators
    MatrixType& operator=(const MatrixType& other)
    {
        if (&other == this) { return *this; }
        m_storage = other.m_storage;
    }

    MatrixType& operator=(MatrixType&& other) noexcept
    {
        if (&other == this) { return *this; }
        m_storage = std::move(other.m_storage);
    }

    template <
        typename... Args,
        typename = enable_if_integral_variadic_t<Args...>
    >
    TValue& operator ()(Args&&... indexes)
    {
       return at(std::forward<Args>(indexes)...);
    }

    template <
        typename... Args,
        typename = enable_if_integral_variadic_t<Args...>
    >
    const TValue& operator ()(Args&&... indexes) const
    {
       return at(std::forward<Args>(indexes)...);
    }

    auto operator[](size_t index)
    {
        KeyType key;
        key[0] = index;
        return Indexer<DIM_SIZE - 1>(key, *this);
    }

    auto operator[](size_t index) const
    {
        KeyType key;
        key[0] = index;
        return Indexer<DIM_SIZE - 1>(key, *this);
    }

private:
    TValue& get_value(const KeyType& key)
    {
        auto search = m_storage.find(key);
        if (search == m_storage.end())
        {
            // ТУТ ПРОИСХОДИТ ДИЧЬ, default_value можно модифицировать
//            return DEFAULT_VALUE;
            return default_value;
        }

        return search->second;
    }

    Storage m_storage;
};
