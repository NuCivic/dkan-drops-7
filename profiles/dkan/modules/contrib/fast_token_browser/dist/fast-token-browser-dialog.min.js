(function ($, Drupal, window, document, undefined) {

  'use strict';

  Drupal.behaviors.tokenBrowserDialog = {
    attach: function (context, settings) {
      var $window = $(window, context);
      var $links = $('a.token-browser').once('token-browser');

      var data = {
        'ajax_page_state[theme]': settings.ajaxPageState.theme,
        'ajax_page_state[theme_token]': settings.ajaxPageState.theme_token
      };

      var options = {
        'title': Drupal.t('Token Browser'),
        'classes': { 'ui-dialog': 'token-browser-dialog' },
        'dialogClass': 'token-browser-dialog',
        'width': Math.min(1500, $window.width() * 0.8),
        'close': function () {
          $(this).remove();
          $links.removeClass('token-browser-open');
        }
      };

      $links.click(function (event) {
        var $dialog = $('<div>').hide();
        var url = $(event.target).attr('href');

        event.stopPropagation();
        event.preventDefault();

        if ($links.hasClass('token-browser-open')) {
          return false;
        }

        $links.addClass('token-browser-open');
        $dialog.addClass('loading').appendTo('body');
        $dialog.dialog(options);
        $dialog.load(url, data, function () { $dialog.removeClass('loading'); });
      });
    }
  };

})(jQuery, Drupal, this, this.document);
