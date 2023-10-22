MATRIX_TEMPLATE_ARGS
typename MATRIX_TYPE::
MatrixType& MATRIX_TYPE::operator=(const MatrixType& other)
{
    if (&other == this) { return *this; }
    m_storage = other.m_storage;
}

MATRIX_TEMPLATE_ARGS
typename MATRIX_TYPE::
MatrixType& MATRIX_TYPE::operator=(MatrixType&& other) noexcept
{
    if (&other == this) { return *this; }
    m_storage = std::move(other.m_storage);
}

MATRIX_TEMPLATE_ARGS
auto MATRIX_TYPE::operator[](size_t index)
{
    KeyType key;
    key[0] = index;
    return Indexer<DIM_SIZE - 1>(key, *this);
}

MATRIX_TEMPLATE_ARGS
auto MATRIX_TYPE::operator[](size_t index) const
{
    KeyType key;
    key[0] = index;
    return Indexer<DIM_SIZE - 1>(key, *this);
}