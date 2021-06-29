/**
 * @file
 * Requesting and displaying Bibliofil Lists.
 */
(function ($) {

  "use strict";

  Drupal.behaviors.bibliofil_lists = {
    attach: function (context, settings) {
      $("[data-bibliofil-lists-hash]", context).once(function () {
        var list = $(this);
        var hash = list.attr("data-bibliofil-lists-hash");
        var type = list.attr("data-bibliofil-lists-type");
        var wrapper = list.parents(".pane-bibliofil-lists-" + type).hide();

        var url = settings.basePath + settings.pathPrefix + "bibliofil/" + hash + "/" + type;

        // @TODO: Move this to Drupal.ajax.
        $.get(url, {}, function (response) {
            if (response) {
              list.replaceWith(response);
              wrapper.slideDown("fast");
              Drupal.attachBehaviors(context);
            }
          }
        );
      });

      if ($('.carousel-tabs .carousel-select').length && !$('div#select-inner select').length) {
        $('select.carousel-select').wrap('<div id=select-inner></div>');
      }
    }
  };
}(jQuery));
