namespace art::seafire::resources
{

  void
  handle(server::request_t& req,
         server::response_t& res,
         Resource auto const& r)
  {
    using resource_traits = traits::resource_traits<std::decay_t<decltype(r)>>;

    auto const not_allowed = [&req, &res]
    {
      set<protocol::rfc7231::allow_t>(res, resource_traits::allowed_methods());
      res.send(server::common_error_t::method_not_allowed);
    };

    auto const& method = req.get_message().method();

    // GET
    //
    if (method == "GET" || method == "HEAD") {
      if constexpr (resource_traits::is_gettable) {
        negotiate<typename resource_traits::get_representation_type>(
          req,
          res,
          [&req, &res, &r](std::optional<protocol::media_type_t> const& accepted_type)
          {
            handle_get(
              req,
              res,
              r,
              accepted_type,
              req.get_message().method() == "HEAD" ? get_kind_t::head : get_kind_t::get
            );
          }
        );

        return;
      }
      else {
        not_allowed();
      }
    }

    // PUT (update)
    //
    else if (method == "PUT") {
      if constexpr (resource_traits::is_updatable) {
        if constexpr (std::is_same_v<void, typename resource_traits::update_result_type>) {
          handle_update(req, res, r, std::nullopt);
        }
        else if constexpr (representation::Representation<typename resource_traits::update_result_type>) {
          negotiate<typename resource_traits::update_result_type>(
            req,
            res,
            [&req, &res, &r](std::optional<protocol::media_type_t> const& accepted_type)
            {
              handle_update(req, res, r, accepted_type);
            }
          );
        }
        else {
          negotiate<typename traits::resource_traits<typename resource_traits::update_result_type>::get_representation_type>(
            req,
            res,
            [&req, &res, &r](std::optional<protocol::media_type_t> const& accepted_type)
            {
              handle_update(req, res, r, accepted_type);
            }
          );
        }
      }
      else {
        not_allowed();
      }
    }

    // POST (create)
    //
    else if (method == "POST") {
      if constexpr (resource_traits::is_creatable) {
        if constexpr (std::is_same_v<void, typename resource_traits::update_result_type>) {
          handle_create(req, res, r, std::nullopt);
        }
        else if constexpr (representation::Representation<typename resource_traits::create_result_type>) {
          negotiate<typename resource_traits::create_result_type>(
            req,
            res,
            [&req, &res, &r](std::optional<protocol::media_type_t> const& accepted_type)
            {
              handle_create(req, res, r, accepted_type);
            }
          );
        }
        else {
          using one = typename resource_traits::create_result_type;
          using two = typename traits::resource_traits<one>::get_representation_type;
          //using resource_traits = traits::resource_traits<
          negotiate<two>(
            req,
            res,
            [&req, &res, &r](std::optional<protocol::media_type_t> const& accepted_type)
            {
              handle_create(req, res, r, accepted_type);
            }
          );
        }
      }
      else {
        not_allowed();
      }
    }

    // DELETE
    //
    else if (method == "DELETE") {
      if constexpr (resource_traits::is_erasable) {
        handle_erase(req, res, r);
      }
      else {
        not_allowed();
      }
    }

    // method not implemented.
    //
    else {
      res.send(server::common_error_t::not_implemented);
    }
  }

} // namespace art::seafire::resources
