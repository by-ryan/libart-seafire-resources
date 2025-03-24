namespace art::seafire::resources
{

  template<GettableResource R>
  void
  handle_get(server::request_t& req,
             server::response_t& res,
             R const& resource,
             std::optional<protocol::media_type_t> const& accepted_type,
             get_kind_t kind)
  {
    auto rep = common::invoke(resource, req, &R::get);
    auto selected_rep = representation::select(rep, accepted_type);

    if (!check_preconditions(req, res, selected_rep))
      return;

    namespace rfc7231 = protocol::rfc7231;
    namespace rfc7232 = protocol::rfc7232;

    if (auto opt_val = get_etag(resource); opt_val)
      set<rfc7232::etag_t>(res, *opt_val);

    if (auto opt_val = get_last_modified(resource); opt_val)
      set<rfc7232::last_modified_t>(res, *opt_val);

    if (kind == get_kind_t::created_resource || kind == get_kind_t::updated_resource) {
      if (auto opt_val = get_location(resource); opt_val)
        set<rfc7231::location_t>(res, *opt_val);
    }

    protocol::status_code_t status{200};

    if (kind == get_kind_t::created_resource)
      status = 201;

    bool const send_content = kind != get_kind_t::head;
    representation::send(req, res, status, selected_rep, send_content);
  }

} // namespace art::seafire::resources
