namespace art::seafire::resources
{

  template<CreatableResource R>
  void
  handle_create(server::request_t& req,
                server::response_t& res,
                R const& resource,
                std::optional<protocol::media_type_t> const& accepted_type)
  {
    using resource_traits = traits::resource_traits<std::decay_t<R>>;
    using input_representation_type = typename resource_traits::create_input_representation_type;

    using t = traits::resource_traits<R>;

    if constexpr (std::is_same_v<void, typename t::create_result_type>) {
      auto content_type = get<protocol::rfc7231::content_type_t>(req);
      auto input = input_representation_type::read_from(content_type, req.content());
      common::invoke(resource, req, &R::create, input);
      res.send(204);
    }
    else if constexpr (representation::traits::is_variant_input_representation_v<input_representation_type>) {
      res.send(server::common_error_t::not_implemented);
    }
    else if constexpr (representation::Representation<typename t::create_result_type>) {
      auto content_type = get<protocol::rfc7231::content_type_t>(req);
      auto input = input_representation_type::read_from(content_type, req.content());
      auto result = common::invoke(resource, req, &R::create, input);
      auto selected_rep = representation::select(result, accepted_type);
      representation::send(req, res, 201, selected_rep, true);
    }
    else {
      auto content_type = get<protocol::rfc7231::content_type_t>(req);
      auto input = input_representation_type::read_from(content_type, req.content());
      auto result = common::invoke(resource, req, &R::create, input);
      handle_get(req, res, result, accepted_type, get_kind_t::created_resource);
    }
  }

} // namespace art::seafire::resources
