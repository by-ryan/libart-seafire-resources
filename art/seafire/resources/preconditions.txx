namespace art::seafire::resources
{

  bool
  check_preconditions(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& r)
  {
    namespace rfc7232 = protocol::rfc7232;

    using representation_traits = representation::traits::representation_traits<decltype(r)>;

    if (auto if_match = get<rfc7232::if_match_t>(req); if_match) {
      if constexpr (representation_traits::has_entity_tag) {
        bool match = check_if_match(req, res, r, *if_match);

        if (!match) {
          res.send(server::common_error_t::precondition_failed);
          return false;
        }
      }
      else {
        res.send(server::common_error_t::precondition_failed);
        return false;
      }
    }
    else if (auto if_unmodified_since = get<rfc7232::if_unmodified_since_t>(req); if_unmodified_since) {
      bool match = check_if_unmodified_since(req, res, r, *if_unmodified_since);

      if (!match) {
        res.send(server::common_error_t::precondition_failed);
        return false;
      }
    }

    if (auto if_none_match = get<rfc7232::if_none_match_t>(req); if_none_match) {
      bool match = false;

      if (!match) {
        if (req.get_message().method() == "GET" || req.get_message().method() == "HEAD")
          res.send(server::common_error_t::not_modified);
        else
          res.send(server::common_error_t::precondition_failed);
        return false;
      }
    }
    else if (auto if_modified_since = get<rfc7232::if_modified_since_t>(req); if_modified_since) {
      bool match = false;

      if (!match) {
        res.send(server::common_error_t::precondition_failed);
        return false;
      }
    }

    return true;
  }

  // If-Match
  //
  bool
  check_if_match(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& r,
    protocol::rfc7232::if_match_t const& if_match)
  {
    using representation_traits = representation::traits::representation_traits<std::decay_t<decltype(r)>>;

    if constexpr (representation_traits::has_entity_tag) {
      auto etag = [&r]
      {
        if constexpr (common::traits::is_optional_v<decltype(r.etag())>) {
          return r.etag();
        }
        else {
          return std::optional<protocol::rfc7232::entity_tag_t>{r.etag()};
        }
      }();

      if (!etag)
        return false;

      if (if_match.is_anything())
        return true;

      for (auto const& j : if_match.tags()) {
        if (strong_compare(*etag, j))
          return true;
      }
    }

    return false;
  }

  // If-Unmodified-Since
  //
  bool
  check_if_unmodified_since(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& r,
    std::chrono::system_clock::time_point const& if_unmodified_since)
  {
    using representation_traits = representation::traits::representation_traits<std::decay_t<decltype(r)>>;

    if constexpr (representation_traits::has_last_modified) {
      auto last_modified = r.last_modified();

      if (last_modified <= if_unmodified_since)
        return true;
    }

    return false;
  }

  // If-None-Match
  //
  bool
  check_if_none_match(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& r,
    protocol::rfc7232::if_none_match_t const& if_none_match)
  {
    return false;
  }

  // If-Modified-Since
  //
  bool
  check_if_modified_since(
    server::request_t& req,
    server::response_t& res,
    representation::BasicRepresentation auto const& r,
    std::chrono::system_clock::time_point const& if_modified_since)
  {
    using representation_traits = representation::traits::representation_traits<std::decay_t<decltype(r)>>;

    if constexpr (representation_traits::has_last_modified) {
      auto last_modified = r.last_modified();

      if (last_modified >= if_modified_since)
        return true;
    }

    return false;
  }

} // namespace art::seafire::resources
