namespace MajSimai
{
    template <typename TView, typename TUnmanaged>
    ArrayView<TView, TUnmanaged>::ArrayView(const TUnmanaged *data, size_t size)
        : m_data(data), m_size(size)
    {
    }

    template <typename TView, typename TUnmanaged>
    size_t ArrayView<TView, TUnmanaged>::Size() const
    {
        return m_size;
    }

    template <typename TView, typename TUnmanaged>
    bool ArrayView<TView, TUnmanaged>::IsEmpty() const
    {
        return m_size == 0;
    }

    template <typename TView, typename TUnmanaged>
    TView ArrayView<TView, TUnmanaged>::operator[](size_t index) const
    {
        return TView(&m_data[index]);
    }

    // ע�⣺����Ƕ��������Ҫ���� typename
    template <typename TView, typename TUnmanaged>
    typename ArrayView<TView, TUnmanaged>::Iterator ArrayView<TView, TUnmanaged>::begin() const
    {
        return Iterator(m_data);
    }

    template <typename TView, typename TUnmanaged>
    typename ArrayView<TView, TUnmanaged>::Iterator ArrayView<TView, TUnmanaged>::end() const
    {
        return Iterator(m_data + m_size);
    }

    template <typename TView, typename TUnmanaged>
    ArrayView<TView, TUnmanaged>::Iterator::Iterator(const TUnmanaged *p)
        : ptr(p)
    {
    }

    template <typename TView, typename TUnmanaged>
    TView ArrayView<TView, TUnmanaged>::Iterator::operator*() const
    {
        return TView(ptr);
    }

    template <typename TView, typename TUnmanaged>
    typename ArrayView<TView, TUnmanaged>::Iterator &ArrayView<TView, TUnmanaged>::Iterator::operator++()
    {
        ++ptr;
        return *this;
    }

    template <typename TView, typename TUnmanaged>
    bool ArrayView<TView, TUnmanaged>::Iterator::operator!=(const Iterator &other) const
    {
        return ptr != other.ptr;
    }
}