#!/bin/bash

set -exv

PROJECT_PATH="$PWD"


PUBLISH_BRANCHES="master develop"
if [ "$TRAVIS_REPO_SLUG" == "realazthat/sgfxapi" ] && [ "$TRAVIS_PULL_REQUEST" == "false" ] && [[ $PUBLISH_BRANCHES =~ "$TRAVIS_BRANCH" ]]; then

  echo -e "Publishing docs/sphinx/html...\n"

  DOCS_PATH="$PWD/build/docs/sphinx/html/"
  #cp -R build/docs/sphinx/html $HOME/sphinx-docs-latest

  cd $HOME
  #git config --global user.email "travis@travis-ci.org"
  #git config --global user.name "travis-ci"
  
  rm -rf gh-pages
  git clone --quiet --branch=gh-pages https://${GH_TOKEN}@github.com/realazthat/sgfxapi gh-pages > /dev/null

  cd gh-pages
  git rm -rf --ignore-unmatch "./$TRAVIS_BRANCH/sphinx-docs"
  mkdir -p "./$TRAVIS_BRANCH/sphinx-docs"
  cp -Rf "$DOCS_PATH" "./$TRAVIS_BRANCH/sphinx-docs/"
  git add -f .
  git -c user.email="travis@travis-ci.org" -c user.name="travis-ci" \
        commit -m "Latest sphinx-docs on successful travis build $TRAVIS_BUILD_NUMBER auto-pushed to gh-pages/$TRAVIS_BRANCH"
  git push -fq origin gh-pages > /dev/null

  echo -e "Published docs/sphinx/html to gh-pages.\n"
  
fi
