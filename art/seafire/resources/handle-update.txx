namespace art::seafire::resources
{

  template<UpdatableResource R>
  void
  handle_update(server::request_t& req,
                server::response_t& res,
                R const& resource,
                std::optional<protocol::media_type_t> const& accepted_type)
  {
    using resource_traits = traits::resource_traits<std::decay_t<R>>;
    using input_representation_type = typename resource_traits::update_input_representation_type;

    auto content_type = get<protocol::rfc7231::content_type_t>(req);

    if (!content_type) {
      res.send(400);
      return;
    }

    if (!input_representation_type::can_accept_input(*content_type)) {
      res.send(400);
      return;
    }

    auto input = input_representation_type::read_from(content_type, req.content());

    using t = traits::resource_traits<R>;

    if constexpr (GettableResource<typename t::update_result_type>) {
      auto result = common::invoke(resource, req, &R::update, input);
      handle_get(req, res, result, accepted_type, get_kind_t::updated_resource);
    }
    else if constexpr (representation::Representation<typename t::update_result_type>) {
      auto result = common::invoke(resource, req, &R::update, input);
      auto selected_rep = representation::select(result, accepted_type);
      representation::send(req, res, 200, selected_rep, true);
    }
    else {
      common::invoke(resource, req, &R::update, input);
      res.send(204);
    }
  }

} // namespace art::seafire::resources
