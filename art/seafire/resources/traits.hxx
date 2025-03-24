#ifndef art__seafire__resources__traits_hxx_
#define art__seafire__resources__traits_hxx_

#include <art/seafire/common/traits.hxx>
#include <art/seafire/protocol/media-type.hxx>
#include <art/seafire/protocol/rfc7232/entity-tag.hxx>
#include <art/seafire/protocol/token.hxx>
#include <art/seafire/representation/traits.hxx>

#include <art/uri/uri.hxx>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

namespace art::seafire::resources::traits
{

  // =============================================================================
  // resources.
  //

  // is_gettable_resource<R>
  //

  template<typename, typename = std::void_t<>>
  struct is_gettable_resource : std::false_type {};

  template<typename R>
  struct is_gettable_resource<
    R,
    std::void_t<
      decltype(&R::get)
    >
  > : std::bool_constant<
    std::is_same_v<
      void,
      typename common::traits::function_traits<decltype(&R::get)>::return_type
    >
    ||
    representation::traits::is_representation_v<
      typename common::traits::function_traits<decltype(&R::get)>::return_type
    >
  > {
  };

  template<typename R>
  inline constexpr bool is_gettable_resource_v{is_gettable_resource<R>::value};

  template<typename R, bool = is_gettable_resource_v<R>>
  struct get_traits {
    using result_type = void;
  };

  template<typename R>
  struct get_traits<R, true> {
    using function_traits = common::traits::function_traits<decltype(&R::get)>;
    using result_type = common::traits::remove_optional_t<typename function_traits::return_type>;
  };

  // is_updatable_resource<R>
  //

  template<typename, typename = std::void_t<>>
  struct is_updatable_resource : std::false_type {};

  template<typename R>
  struct is_updatable_resource<
    R,
    std::void_t<
      decltype(&R::update)
    >
  > : std::bool_constant<
    // Check if the first arg to R::update() is an input representation.
    representation::traits::is_input_representation_v<
      typename common::traits::first_arg<decltype(&R::update)>::type
    >
    &&
    (
      // Check if return value of R::update() is void.
      std::is_same_v<
        void,
        typename common::traits::function_traits<decltype(&R::update)>::return_type
      >
      ||
      // Check if return value of R::update() is a gettable resource.
      is_gettable_resource_v<
        typename common::traits::function_traits<decltype(&R::update)>::return_type
      >
      ||
      // Check if return value of R::update() is a representation.
      representation::traits::is_representation_v<
        typename common::traits::function_traits<decltype(&R::update)>::return_type
      >
    )
  > {
  };

  template<typename R>
  inline constexpr bool is_updatable_resource_v{is_updatable_resource<R>::value};

  template<typename R, bool = is_updatable_resource_v<R>>
  struct update_traits {
    using input_representation_type = void;
    using result_type = void;
  };

  template<typename R>
  struct update_traits<R, true> {
    using function_traits = common::traits::function_traits<decltype(&R::update)>;
    using input_representation_type = common::traits::first_arg_t<decltype(&R::update)>;
    using result_type = typename function_traits::return_type;
  };

  // is_creatable_resource<R>
  //

  template<typename, typename = std::void_t<>>
  struct is_creatable_resource : std::false_type {};

  template<typename R>
  struct is_creatable_resource<
    R,
    std::void_t<
      decltype(&R::create)
    >
  > : std::bool_constant<
    representation::traits::is_input_representation_v<
      common::traits::first_arg_t<decltype(&R::create)>
    >
    &&
    (
      // Check if return value of R::create() is void.
      std::is_same_v<
        void,
        typename common::traits::function_traits<decltype(&R::create)>::return_type
      >
      ||
      // Check if return value of R::create() is a gettable resource.
      is_gettable_resource_v<
        typename common::traits::function_traits<decltype(&R::create)>::return_type
      >
      ||
      // Check if return value of R::create() is a representation.
      representation::traits::is_representation_v<
        typename common::traits::function_traits<decltype(&R::create)>::return_type
      >
    )
  > {
  };

  template<typename R>
  inline constexpr bool is_creatable_resource_v{is_creatable_resource<R>::value};

  template<typename R, bool = is_creatable_resource_v<R>>
  struct create_traits {
    using input_representation_type = void;
    using result_type = void;
  };

  template<typename R>
  struct create_traits<R, true> {
    using function_traits = common::traits::function_traits<decltype(&R::create)>;
    using input_representation_type = common::traits::first_arg_t<decltype(&R::create)>;
    using result_type = typename function_traits::return_type;
  };

  // is_erasable_resource<R>
  //

  template<typename, typename = std::void_t<>>
  struct is_erasable_resource : std::false_type {};

  template<typename R>
  struct is_erasable_resource<
    R,
    std::void_t<
      decltype(std::declval<R const>().erase())
    >
  > : std::true_type{
  };

  template<typename R>
  constexpr bool is_erasable_resource_v{is_erasable_resource<R>::value};

  // is_resource
  //

  template<typename R>
  struct is_resource : std::bool_constant<
    is_gettable_resource_v<R> ||
    is_updatable_resource_v<R> ||
    is_creatable_resource_v<R> ||
    is_erasable_resource_v<R>
  > {
  };

  template<typename R>
  constexpr bool is_resource_v{is_resource<R>::value};

  // has_location
  //

  template<typename, typename = std::void_t<>>
  struct has_location : std::false_type {};

  template<typename R>
  struct has_location<
    R,
    std::void_t<
      decltype(
        uri::uri_t{
          std::declval<R const>().location()
        }
      )
    >
  > : std::true_type{
  };

  template<typename R>
  constexpr bool has_location_v{has_location<R>::value};

  // resource_traits
  //

  template<typename R>
  struct resource_traits {
    using resource_type = R;

    // properties
    //

    static constexpr bool has_entity_tag{
      representation::traits::has_entity_tag_v<resource_type>
    };

    static constexpr bool has_last_modified{
      representation::traits::has_last_modified_v<resource_type>
    };

    static constexpr bool has_location{
      has_location_v<resource_type>
    };

    static
    protocol::tokens_t
    allowed_methods()
    {
      protocol::tokens_t methods;

      if constexpr (is_gettable) {
        methods.emplace_back("GET");
        methods.emplace_back("HEAD");
      }

      if constexpr (is_updatable)
        methods.emplace_back("PUT");

      if constexpr (is_creatable)
        methods.emplace_back("POST");

      if constexpr (is_erasable)
        methods.emplace_back("DELETE");

      return methods;
    }

    // GET
    //
    static constexpr bool is_gettable{
      is_gettable_resource_v<resource_type>
    };

    using get_traits = traits::get_traits<resource_type>;
    using get_representation_type = typename get_traits::result_type;
    using get_representation_type_traits = representation::traits::representation_traits<
      get_representation_type
    >;

    // UPDATE
    //
    static constexpr bool is_updatable{
      is_updatable_resource_v<resource_type>
    };

    using update_traits = traits::update_traits<resource_type>;
    using update_input_representation_type = typename update_traits::input_representation_type;
    using update_input_representation_type_traits = representation::traits::input_representation_traits<update_input_representation_type>;
    using update_result_type = typename update_traits::result_type;

    // CREATE
    //
    static constexpr bool is_creatable{
      is_creatable_resource_v<resource_type>
    };

    using create_traits = traits::create_traits<resource_type>;
    using create_input_representation_type = typename create_traits::input_representation_type;
    using create_input_representation_type_traits = representation::traits::input_representation_traits<create_input_representation_type>;
    using create_result_type = typename create_traits::result_type;

    // ERASE
    //
    static constexpr bool is_erasable{
      is_erasable_resource_v<resource_type>
    };
  };

} // namespace art::seafire::resources::traits

#endif
