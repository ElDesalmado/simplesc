﻿#pragma once

#include "simplecs/c_api/types.h"
#include "simplecs/config.hpp"
#include "simplecs/impl/id_pool.h"

#include "simplecs/generic/selector.h"
#include "simplecs/impl/selector.h"

#include <set>
#include <unordered_map>

template<>
struct std::hash<simplecs::c_api::entity_descriptor>
{
    size_t operator()(const simplecs::c_api::entity_descriptor &descriptor) const
    {
        return std::hash<size_t>{}(descriptor.handle.h);
    }
};

namespace simplecs::c_core
{
    /**
     * Naive implementation for C API.
     */
    class relational_table
    {
    public:
        using entity_handle_type = simplecs::c_api::entity_descriptor;
        using component_handle_type = simplecs::c_api::component_descriptor;
        using component_type = c_api::type_descriptor;
        using selection_handle = c_api::handle;

        void register_components(const entity_handle_type &owningEntity,
                                 const component_handle_type *array,
                                 size_t length,
                                 c_api::reg_error * /*results*/);

        void unregister_components(const entity_handle_type &owningEntity,
                                   const component_type *array,
                                   size_t length,
                                   c_api::unreg_error * /*results*/);

        c_api::entity_selection select_entities(
            const std::vector<component_type> &componentsToSelect);

        void free_selection(const selection_handle& handle);

    private:
        std::unordered_map<component_type, std::set<entity_handle_type>> componentToEntityMap_;

        // maps an entity to a particular component
        std::unordered_map<entity_handle_type, component_handle_type> entityToComponentMap_;
        std::unordered_map<selection_handle, std::vector<entity_handle_type>> selections_;

        simplecs::detail::id_pool<size_t> selectionHandlePool_;
        generic::selector<impl::selector_std<entity_handle_type, component_type>> selector_;
    };


}   // namespace simplecs::c_core

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/c_api/src/relational_table.ipp"
#endif
